#
# coroutine бол thread-тэй харьцуулбал context switch хийхдээ
# арай хурдан cooperative concurrency хэрэгсэл юм, мөн thread-тэй
# харьцуулбал дундын нөөц булаалдахгүй тул програмчлахад бас хялбар
# гол давуу тал нь удаан ажиллагаатай функц дээр context switch 
# хийлгэх байдлаар бусад task-уудыг ажиллуулах боломжтой, дээрээс нь 
# олон цөмтэй CPU-ийн үр ашгийг нээх гаргахгүй л байх, орчин үеийн
# компютер гэдэг тэр чигтээ asynchronous буюу бөөн delay хүлээлгийн
# горим бүхий техникүүдийн цуглуулга юм, өөрөөр хэлбэл диксрүү уншилт
# бичилт хийх, вэб сайтруу хүсэлт илгээх, сүлжээгээр тэр хүсэлтийг
# хүлээх гэх мэт delay үүсгэх нөхцөл байдал байнга гарч байдаг,
# иймэрхүү тохиолдолд context switch хийчээд бусад хэрэгцээтэй 
# байж болох тооцооллууд дээр CPU нөөцөө хэрэглэвэл нийт бүтээмж нь
# дагаад өсдөг гэсэн үг.
#
#

import time
import random
import asyncio
import uvloop



# Төсөөллийн SoA ECS бүтэц
ENTITY_SIZE      = 50000
entity_positions = [
    [random.uniform(0, 100), random.uniform(0, 100)]
    for i in range(0, ENTITY_SIZE)
    ]
entity_velocities = [
    [random.uniform(0, 5), random.uniform(0, 5)]
    for i in range(0, ENTITY_SIZE)
]


# Өгөгдсөн векторын дагуу хөдөлгөөн хийх төсөөллийн систем
async def move_task(element):
    entity_idx = element[0]
    entity     = element[1]
    await asyncio.sleep(0) # contex switching
    position   = entity [0]
    velocity   = entity [1]
    await asyncio.sleep(0) # contex switching
    new_position_x = position[0]+velocity[0]
    new_position_y = position[1]+velocity[1]
    return (entity_idx, (new_position_x, new_position_y))

async def run():
    coroutine_tasks = [
        move_task((entity_idx, entity))
        for entity_idx, entity
        in enumerate(zip(entity_positions, entity_velocities))
    ]
    new_positions = await asyncio.gather(*coroutine_tasks)
    # Шинэ байрлалуудын дагуу өмнөх утгуудаа шинэчилэх
    for entity_idx, position in new_positions:
        entity_positions[entity_idx][0] = position[0]
        entity_positions[entity_idx][1] = position[1]
        pass


if __name__ == '__main__':
    start_time = time.time()
    uvloop.install()

    # Төсөөллийн тоглоомын цикл
    for i in range(20):
        loop = asyncio.get_event_loop()
        loop.run_until_complete(run())
        print(f"DEBUG position<x={entity_positions[0][0]}, y={entity_positions[0][1]}>")

    print(f"elapsed time to process {ENTITY_SIZE} entities : {time.time()-start_time}")
