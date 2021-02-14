import json
import numpy as np
import cv2
import matplotlib.pyplot as plt
from evaluate.lane import LaneEval

json_gt = [json.loads(line) for line in open('true.json')]
json_pred = [json.loads(line) for line in open('res.json').readlines()]

key = {}
cnt = 0
for i in json_gt:
    key[i['raw_file']] = cnt
    cnt += 1

json_pred = json_pred[0]
for i in range(0, 100):
    pred = json_pred[i]
    pred_lanes = pred['lanes']
    run_time = pred['run_time']
    raw_file = pred['file_name']

    gt = json_gt[key[raw_file]]

    gt_lanes = gt['lanes']
    y_samples = gt['h_samples']

    img = plt.imread(raw_file)
    gt_lanes_vis = [[(x, y) for (x, y) in zip(lane, y_samples) if x >= 0] for lane in gt_lanes]
    pred_lanes_vis = [[(x, y) for (x, y) in zip(lane, y_samples) if x >= 0] for lane in pred_lanes]
    img_vis = img.copy()

    for lane in gt_lanes_vis:
        cv2.polylines(img_vis, np.int32([lane]), isClosed=False, color=(0, 255, 0), thickness=5)
    for lane in pred_lanes_vis:
        cv2.polylines(img_vis, np.int32([lane]), isClosed=False, color=(0, 0, 255), thickness=2)

    plt.imshow(img_vis)
    plt.show()

    np.random.shuffle(pred_lanes)
    print(i, raw_file, LaneEval.bench(pred_lanes, gt_lanes, y_samples, run_time))
