#
# хэдий дараалсан горимоор тооцоолол хийж байгаа боловч multiprocessing-тэй
# харьцуулбал маш түргэн ажиллаж байгааг эндээс анзаарч болно
#
# зүйрэлбэл боловсролын бизнес эрхлэхийн тулд барилгаа ганц удаа барьж аваад
# нэг багшаар бүх хичээлүүдээ орохыг оролдож байна л гэсэн үг
#
#

import time
import random
import ctypes
import multiprocessing


# Төсөөллийн SoA ECS бүтэц
ENTITY_SIZE      = 1000000
entity_positions = [
    [random.uniform(0, 100), random.uniform(0, 100)]
    for i in range(0, ENTITY_SIZE)
    ]
entity_velocities = [
    [random.uniform(0, 5), random.uniform(0, 5)]
    for i in range(0, ENTITY_SIZE)
]


# Өгөгдсөн векторын дагуу хөдөлгөөн хийх төсөөллийн систем
def move_task(element):
    entity_idx = element[0]
    entity     = element[1]

    position   = entity [0]
    velocity   = entity [1]

    new_position_x = position[0]+velocity[0]
    new_position_y = position[1]+velocity[1]

    return (entity_idx, (new_position_x, new_position_y))


if __name__ == '__main__':
    start_time = time.time()

    # Төсөөллийн тоглоомын цикл
    for i in range(20):
        new_positions = []
        for entity_idx, entity in enumerate(zip(entity_positions, entity_velocities)):
            new_positions.append(move_task((entity_idx, entity)))

        # Шинэ байрлалуудын дагуу өмнөх утгуудаа шинэчилэх
        for entity_idx, position in new_positions:
            entity_positions[entity_idx][0] = position[0]
            entity_positions[entity_idx][1] = position[1]
            pass
        print(f"DEBUG position<x={entity_positions[0][0]}, y={entity_positions[0][1]}>")

    print(f"elapsed time to process {ENTITY_SIZE} entities : {time.time()-start_time}")
    
