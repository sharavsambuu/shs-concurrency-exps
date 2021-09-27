#
# Python хэлний хувьд олон CPU цөмүүдийн үр ашгийг гаргахын 
# тулд multiprocessing-г coroutine-уудтай хослуулж болно
#
# тэр нэгж эгэл боловсруулалт буюу coroutine нь миний бодож 
# байгаагаар fiber гэдэг эгэл(primitive) болно
#
# M:N эффект гэх юмуудаа M цөм дээр N ширхэг эгэл зэргэлдээ боловсруулалт
# гэсэн үг. Яг техник хангамж дээрхи параллельчилал түвшин нь M болно
#
# яг энэ fiber primitive мөн эсэх талаар жаахан эргэлзэж байгаач
# санаа нь бол иймэрхүү, дараах холбоосоос fiber-ийн талаар 
# дэлгэрэнгүй мэдэж болно
#
# - https://www.gdcvault.com/play/1022186/Parallelizing-the-Naughty-Dog-Engine
#
#

import time
import random
import asyncio
import multiprocessing


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
    position   = entity [0]
    velocity   = entity [1]

    await asyncio.sleep(0) # contex switching

    new_position_x = position[0]+velocity[0]
    new_position_y = position[1]+velocity[1]

    return [entity_idx, [new_position_x, new_position_y]]


async def fibers_run(data):
    coroutine_tasks = [
        move_task((entity_idx, entity))
        for entity_idx, entity
        in data
    ]
    return await asyncio.gather(*coroutine_tasks)


def process_run(data):
    loop    = asyncio.get_event_loop()
    return loop.run_until_complete(fibers_run(data))


if __name__ == '__main__':
    start_time = time.time()

    # процессуудад өгөгдөл тарааж хувиарлах
    N_SPLIT  = 3000
    do_chunk = lambda task_data, x: [list(task_data[i:i+x]) for i in range(0, len(task_data), x)]

    # Төсөөллийн тоглоомын цикл
    for i in range(20):
        with multiprocessing.Pool(4) as p:
            task_data = [
                [entity_idx, list(entity)]
                for entity_idx, entity
                in enumerate(zip(entity_positions, entity_velocities))
            ]
            chunks        = do_chunk(task_data, N_SPLIT)
            new_positions = p.map(process_run, chunks)
            new_positions = [x for xs in new_positions for x in xs]

        # Шинэ байрлалуудын дагуу өмнөх утгуудаа шинэчилэх
        for entity_idx, position in new_positions:
            entity_positions[entity_idx][0] = position[0]
            entity_positions[entity_idx][1] = position[1]
            pass
        print(f"DEBUG position<x={entity_positions[0][0]}, y={entity_positions[0][1]}>")

    print(f"elapsed time to process {ENTITY_SIZE} entities : {time.time()-start_time}")