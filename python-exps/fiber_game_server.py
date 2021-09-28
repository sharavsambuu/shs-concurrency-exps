#
# fiber эгэлээр төсөөллийн тоглоомын сервер ажиллуулах туршилт
# нэг процесс дотор олон тоглоомын сервэр ажиллуулж байна гэсэн үг
#
#

import time
import signal
import os
import sys
import multiprocessing


def init():
    signal.signal(signal.SIGINT, signal.SIG_IGN)


# дотроо тоглоомын өрөөнүүд ажиллуулж байх нэг процесс
def worker_process(queue_in, queue_out):
    process_id = os.getpid()
    print(f"#WORKER_{process_id} : process is spawned")
    while (True):
        while not queue_in.empty():
            msg = queue_in.get()
            if msg.get("COMMAND")=="PING_WORKER":
                print(f"#WORKER_{process_id} : received PING_WORKER command")
                queue_out.put({"MESSAGE": "PONG", "BODY": f"{process_id}"})

        time.sleep(0.01)

        pass
    pass

# worker процессүүдрүү комманд илгээж удирдах зориулалттай процесс
def admin_process(worker_queue_list):
    print("#ADMIN : process is spawned")
    start_time = time.time()

    command_condition_traps = {
        "PING_TO_WORKERS" : [5, False]
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
            ping_workers()
            print("#ADMIN : Pinging to worker processes...")

        if (int(elapsed_time)%10==0) and (command_condition_traps["PING_TO_WORKERS"][1]==True):
            print("#ADMIN : Periodic workers health check")
            ping_workers()

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
        worker_queue_list = []
        for i in range(N_WORKERS):
            worker_queue_in  = manager.Queue()
            worker_queue_out = manager.Queue()
            worker_queue_list.append((worker_queue_in, worker_queue_out))
            pool.apply_async(worker_process, (worker_queue_in, worker_queue_out,))

        pool.apply_async(admin_process, (worker_queue_list,))

        time.sleep(1)

        pool.close()
        pool.join()
    except KeyboardInterrupt:
        pool.terminate()
        pool.join()
    pass