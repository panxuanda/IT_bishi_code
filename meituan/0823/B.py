'''请在仅依赖 numpy/pandas/scikit-learn 的前提下，完成一个基于 DBSCAN 的异常检测器，并对给定测试样本输出聚类结果(正常簇 0,1,2 .;离群点 -1)。
1. 读取数据
train 字段:二维列表，元素均为数值特征
test 字段:二维列表，特征维度与 train 相同
无标签，全为无监督场景
2.预处理将 train 与 test 按行拼接得到整体数据集使用StandardScaler对所有特征做标准化(fit transform 一次完成)
3.DBSCAN 聚类
采用DBSCAN聚类，你可能会用到的参数固定为eps=0.3，min samples=3,metric="euclidean",algorithm="auto"
4.簇标签重映射(唯一化输出)
设原本的标签集合为{-1,0,1,...}，其中 -1 表示离群对所有非 -1 的簇 L
计算簇在标准化特征空间的质心Cl
按质心第一维坐标从小到大排序得到顺序 L0，L1，….
u重新赋值:L0→0，L1→1，…
离群点标签保持 -1 不变
5.结果输出
仅对 test 部分输出重新映射后的标签序列
以单行 JSON 数组 输出
输入描述
标准输入仅一行 JSON，示例:
{"train":[[0,0],[0.1,0],[5,5]]"test":[[0.05,0.05],[9, 0]]}所有数值为整数/浮点数，无空行
输出描述
标准输出仅含一行:[0，-1]
数组长度等于测试样本数
逗号后须有空格，符合 JSON 规范
补充说明
1.标准化:仅用一次 StandardScaler;不要对 train、test分别拟合
2.超参数:eps=0.3,min samples=3 固定
3.为了确保通过测试用例，仅允许使用numpy/pandas/scikit-learn'''

#!/usr/bin/env python3
import sys, json
import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import DBSCAN

def dbscan_anomaly_detector(train, test):
    tr = np.array(train, dtype=float)
    te = np.array(test, dtype=float)
    if tr.ndim == 1:
        tr = tr.reshape(-1, 1)
    if te.ndim == 1:
        te = te.reshape(-1, 1)
    all_data = np.vstack([tr, te])
    scaler = StandardScaler()
    all_scaled = scaler.fit_transform(all_data)
    db = DBSCAN(eps=0.3, min_samples=3, metric="euclidean", algorithm="auto")
    labels = db.fit_predict(all_scaled)
    cluster_labels = [lab for lab in sorted(set(labels)) if lab != -1]
    if not cluster_labels:
        return [-1] * te.shape[0]
    centroids = {lab: all_scaled[labels == lab].mean(axis=0) for lab in cluster_labels}
    sorted_clusters = sorted(cluster_labels, key=lambda L: float(centroids[L][0]))
    mapping = {old: new for new, old in enumerate(sorted_clusters)}
    n_train = tr.shape[0]
    test_labels = labels[n_train:]
    remapped = [(-1 if lab == -1 else mapping[lab]) for lab in test_labels]
    return remapped

if __name__ == "__main__":
    inp = sys.stdin.read().strip()
    data = json.loads(inp)
    res = dbscan_anomaly_detector(data["train"], data["test"])
    # JSON dump with default separators produces ", " after commas
    print(json.dumps(res))
