from hsemotion.facial_emotions import HSEmotionRecognizer
import numpy as np
import cv2
import time
import torch

TESTS = 100

frame_bgr = cv2.imread("img.png")
face_img = cv2.cvtColor(frame_bgr, cv2.COLOR_BGR2RGB)
model_name = 'enet_b0_8_best_afew'
model_name = 'enet_b0_8_va_mtl'
model_name = 'enet_b2_8'
fer = HSEmotionRecognizer(model_name=model_name)
for i in range(100):
    _, _ = fer.predict_emotions(face_img, logits=False)
results = []
for i in range(TESTS):
    start_time=time.time()
    emotion, scores = fer.predict_emotions(face_img, logits=False)
    elapsed_time = time.time() - start_time
    results.append(elapsed_time * 1000)
print(torch.get_num_threads(), ', elapsed:', np.mean(results), "std: ", np.std(results))
