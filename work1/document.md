# SLIC论文阅读报告

## 论文综述（实验原理）

在图像分割中，超像素是由一系列相似特征(如颜色，位置，亮度)等相似特征的像素点组成的小区域。每个像素点保留了该区域总体特征，便于后续图像分割处理。

在 “SLIC Superpixels Compared to State-of-the-Art Superpixel Methods” 论文中，提出了一种获取超像素的方法。通过结合像素点的位置和颜色信息对像素点进行聚类，并用类中心值作为超像素的值。在聚类过程中，限制了聚类时搜索的范围，使得聚类收敛速度加快。在我看来就是利用了图像信息的局部性，在加速了程序运行速度的同时，又能得到不错的结果。

## 算法流程（实验步骤）

实现大致分为三个部分：

1. 超像素初始化
2. 聚类迭代
3. 绘制超像素图像

算法实现难度不高，但需要注意许多细节。

## 实现细节（实验步骤）

### 超像素初始化

#### 色彩空间的转换

论文中，对于颜色特征的计算并非在rgb空间中，而是在CIELAB空间中，这就需要对图像的色彩进行转换，

#### 超像素中心点选取

首先根据超像素数量得到超像素区域为正方形时的面积，

### 聚类迭代

#### 聚类计算区域

在聚类的过程中，与传统的 k-means 聚类不同，SLIC只对类中心边长为2S的正方形范围的点计算距离进行类的更新。这是算法速度快的关键原因。

#### 距离计算

在计算像素点到类中心的距离时，论文结合了颜色和空间特征，进行加权计算，公式为：

$$D = \sqrt{(\frac{d_c}{N_c})^2+(\frac{d_s}{N_s})^2}$$

其中


$$ 
\begin{aligned}
    d_c &= \sqrt{(l_i-l_j)^2+(a_i-a_j)^2+(b_i-b_j)^2}\\
    d_s &=\sqrt{(x_i-x_j)^2+(y_i-y_j)^2}\\
    N_c &= m\\
    N_s &= \sqrt{\frac{N}{K}}\\
\end{aligned}
$$

$d_c$ 为颜色欧式距离，$d_s$为空间欧式距离，$N_c$ 为一个参数m，$N_s$ 为理想超像素边长,$N$为图像像素数量，$K$为指定的超像素数量。

在实际计算中，因为只是比较大小，为了加快计算，使用如下公式计算距离：

$$D = \sqrt{(d_c^2+(\frac{d_s}{N_s})^2N_c^2}$$



### 绘制超像素图像

#### 合并小像素区域



## 实验结果和分析


<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
    MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>
