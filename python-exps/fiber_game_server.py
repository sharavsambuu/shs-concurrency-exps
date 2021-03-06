#
# fiber эгэлээр төсөөллийн тоглоомын сервер ажиллуулах туршилт
# нэг процесс дотор олон тоглоомын сервэр ажиллуулж байна гэсэн үг
#
# Жишээлбэл нэг PC-ны процессор 4-н цөмтэй байна гэж үзье тэгвэл
# процессорын цөм бүрт нь нэг OS процесс ажиллагаанд оруулаад тэр
# процесс бүр нь тус бүрдээ хэдэн мянган тоглоомын өрөөнүүдийг
# cooperative concurrent горимоор stackful coroutine буюу fiber
# ашиглан хэрэгжүүлэх боломжтой юм цаашлаад хэрвээ ZeroMQ ч юм уу
# redis хэрэглээд өрөө тус бүр нь өөрсдийн гэсэн message system 
# хэрэглээд эхэлбэл цаашаагаа системийг бүхэлд нь scale хийх нь
# бараг л хязгааргүй гэсэн үг, нэмж өргөтгөөд дахиад PC холбоод
# ч тавьж болно гэсэн үг.
#
#

import time
import signal
import os
import sys
import uu
import uuid
import asyncio
import multiprocessing
import uvloop



def init():
    signal.signal(signal.SIGINT, signal.SIG_IGN)



# тоглоомын өрөөны логик
async def game_room_fiber():
    room_id         = uuid.uuid4()
    is_room_running = True
    print(f"#ROOM_{room_id} : room is started")

    while is_room_running:
        print(f"#ROOM_{room_id} : working...")
        await asyncio.sleep(3.01)

    pass


# дотроо тоглоомын өрөөнүүд ажиллуулж байх нэг процесс
def worker_process(queue_in, queue_out):
    uvloop.install()

    process_id = os.getpid()
    print(f"#WORKER_{process_id} : process is spawned")

    # 2 ширхэг өрөө
    game_room_fibers = [asyncio.Task(game_room_fiber()) for i in range(2)]

    async def worker_fiber():
        while (True):
            while not queue_in.empty():
                msg = queue_in.get()
                if msg.get("COMMAND")=="PING_WORKER":
                    print(f"#WORKER_{process_id} : received PING_WORKER command")
                    queue_out.put({"MESSAGE": "PONG", "BODY": f"{process_id}"})
            await asyncio.sleep(0.0)

    async def fiber_cycler():
        while True:
            await worker_fiber()
            for game_room in game_room_fibers:
                await game_room
            await asyncio.sleep(0)


    loop = asyncio.get_event_loop()
    loop.run_until_complete(fiber_cycler())

    pass



# worker процессүүдрүү комманд илгээж удирдах зориулалттай процесс
def admin_process(worker_queue_list):
    print("#ADMIN : process is spawned")
    start_time = time.time()

    command_condition_traps = {
        "PING_TO_WORKERS" : [15, False]
    }

    def ping_workers():
        for worker in worker_queue_list:
            worker_queue_in  = worker[0]
            worker_queue_in.put({"COMMAND": "PING_WORKER"})
        pass

    while (True):

        # worker процессуудаас мессеж цуглуулах
        for worker in worker_queue_list:
            worker_queue_out = worker[1]
            while not worker_queue_out.empty():
                msg = worker_queue_out.get()
                if msg.get("MESSAGE")=="PONG":
                    process_id = msg.get("BODY")
                    print(f"#ADMIN : received PONG response from WORKER_{process_id}")

        # комманд илгээх нөхцлүүд
        elapsed_time = time.time()-start_time
        if command_condition_traps["PING_TO_WORKERS"][0]<=elapsed_time and \
            not command_condition_traps["PING_TO_WORKERS"][1]:
            command_condition_traps["PING_TO_WORKERS"][1] = True
            print("#ADMIN : Pinging to worker processes...")
            ping_workers()

        #if (int(elapsed_time)%10==0) and (command_condition_traps["PING_TO_WORKERS"][1]==True):
        #    print("#ADMIN : Periodic workers health check")
        #    ping_workers()

        time.sleep(0.01)
        pass

    pass



# тоглоомын клиэнтийг төсөөлж төлөөлүүлсэн процесс
def game_client():
    pass



# админ удирдлага хийх зориулалттай төсөөллийн клиэнт
def admin_client():
    pass



if __name__=="__main__":

    N_WORKERS = 4
    N_CLIENTS = 100

    pool    = multiprocessing.Pool(N_WORKERS+1, init)
    manager = multiprocessing.Manager()

    try:
        # тоглоомын сервер ажиллуулах зориулалттай процессүүд
        worker_queue_list = []
        for i in range(N_WORKERS):
            worker_queue_in  = manager.Queue()
            worker_queue_out = manager.Queue()
            worker_queue_list.append((worker_queue_in, worker_queue_out))
            pool.apply_async(worker_process, (worker_queue_in, worker_queue_out,))

        # бусад процессүүдээ удирдах зориулалттай админ процесс
        pool.apply_async(admin_process, (worker_queue_list,))

        time.sleep(1)
        pool.close()
        pool.join()
    except KeyboardInterrupt:
        pool.terminate()
        pool.join()

    pass
