import numpy as np


def f(arr):
    arr = np.array(arr)
    diffs = np.diff(arr)  # Находим разности между соседними элементами
    valid_indices = np.where(diffs == 1)[0]  # Индексы, где разность 1
    if valid_indices.size == 0:
        return 0
    # Используем разности индексов для определения непрерывных сегментов
    segments = np.split(valid_indices, np.where(np.diff(valid_indices) > 1)[0] + 1)
    # Вычисляем длины сегментов без явного цикла
    segment_lengths = np.array(list(map(lambda seg: len(seg) + 1, segments)))
    return segment_lengths.max()


arr = [1, 2, 3, 5, 6, 7, 8, 10]
result = f(arr)
print(result)
