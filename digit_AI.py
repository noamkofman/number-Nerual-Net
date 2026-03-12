import numpy as np
import pandas as pd
#import matplotlib.pyplot as plt
import random
# Load CSV with header (label, pixel0..pixel783)
pf = pd.read_csv('train.csv')

correct = 0
wrong = 0
lr = 0.1
bias = np.zeros(10)
# creates 10,784 weights
weights = np.random.randn(10,784) * 0.01
# i think u chnaged this so we loop over all the pixels in the image AND its label
for _, row in pf.iterrows():
    label = int(row["label"])
    pixels = row.drop("label").to_numpy() / 255.0
    score = np.zeros(1) # set every numebr prediction val to 0 
    # find the likelyhood of it being every number from 1-10
    # weight assigned to that specific pixel when evaluating that specific digit 
    # * brightness val at pixel index
    # then assigns the likelyhood val to the sore of the digit being looped over
    score += weights @ pixels + bias
    # then we update the score of the digit by the bias idk why
    prediction = np.argmax(score)
    if prediction != label:
        weights[label] += lr * pixels
        weights[prediction] -= lr * pixels
        bias[label] += lr
        bias[prediction] -= lr

    print("Pred:", prediction, "Actual:", label)
    if prediction == label:
        correct += 1
    else:
        wrong += 1
print(f"Correct: {correct}, Wrong: {wrong}")
np.save("weights.npy", weights)
np.save("bias.npy", bias)