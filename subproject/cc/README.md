# 前言

在 ams-02实验中，使用的是基于硅径迹探测器（Silicon Tracker，Tracker）测量的粒子击中位置重建出的偏转径迹来计算入射粒子的刚度（Rigidity，R）。

Tracker 探测器自上而下总共有 9 层（L1~L9)，其中 L2～L8 位于永磁体内部，L3～L8两两构成上下两面共三对的组合，这 7 层被称为 Inner Tracker。基于粒子入射点进行径迹拟合的模式有四种，一般按顺序为 Inner、Maxspan（使用全部 Tracker）、InnerL1 和 InnerL9，那么基于此计算的粒子刚度 R 也就有对应的四种类型。在本分析中，对于 L9层均有 xy 击中的粒子，使用 Maxspan 模式重建的刚度；对于 L9 层只有 y 击中的粒子，不能使用该层信息来重建刚度：如果该粒子在 L1 层有任意击中，那么使用 InnerL1 模式重建的刚度；如果该粒子在 L1 层也没有击中，那么只能使用 Inner 模式重建的刚度。

在此基础上，利用飞行时间探测器（Time of Flight，TOF）测量的时间顺序，可以判断出粒子的入射方向，结合拟合径迹的偏转方向可以判断出粒子的电荷符号，进而得到重建刚度的正负号。

在 Tracker 的刚度重建中，有一些情况会导致电荷混淆（charge confusion，cc），即该粒子的电荷被判断成相反的符号。情况其一是当粒子的动量过大时，永磁铁的磁场很难使其发生有效偏转，所以对粒子的偏转方向判断错误，导致电荷误判；情况其二是由于粒子在探测器内相互作用和簇射反冲导致（此情况本人未做了解，故暂时不详细展开）。

cc 这一部分的目的就在于矫正这一部分对于正电子数量测量带来的影响。

# cc ratio & real number

对于正电子而言，最后测量的正电子数量中，能被误判为正电子的粒子只能是能通过与其相同的筛选条件，但是发生了电荷误判的电子；同样的，对于电子而言，其发生电荷误判的粒子来源也只能是正电子。

$$
\begin{aligned}
n_{e^+}^{measure} &= n_{e^+}^{nocc}+n_{e^-}^{cc} \\
n_{e^-}^{measure} &= n_{e^-}^{nocc}+n_{e^+}^{cc}
\end{aligned}
$$

那么通过筛选条件的正电子和电子的真实数量可以表示为：

$$
\begin{aligned}
n_{e^+}^{real} &= n_{e^+}^{nocc}+n_{e^+}^{cc} \\
n_{e^-}^{real} &= n_{e^-}^{nocc}+n_{e^-}^{cc}
\end{aligned}
$$

由于正电子被误判为电子，和电子被误判为正电子本质上是同一过程，所以我们假设正负电子发生电荷误判概率是 相同的，由频率估计概率，我们定义电荷误判率（cc Ratio，cc）：

$$
\begin{aligned}
cc &= \frac{n_{e^+}^{cc}}{n_{e^+}^{nocc}+n_{e^+}^{cc}}=\frac{n_{e^+}^{cc}}{n_{e^+}^{real}} \\
   &= \frac{n_{e^-}^{cc}}{n_{e^-}^{nocc}+n_{e^-}^{cc}}=\frac{n_{e^-}^{cc}}{n_{e^-}^{real}}
\end{aligned}
$$

那么，公式 1 里面的四个未知量可以表示为：

$$
\begin{aligned}
n_{e^+}^{measure} &= n_{e^+}^{real}\cdot (1-cc)+n_{e^-}^{real}\cdot cc \\
n_{e^-}^{measure} &= n_{e^-}^{real}\cdot (1-cc)+n_{e^+}^{real}\cdot cc 
\end{aligned}
$$

整理可得：

$$
\begin{aligned}
n_{e^+}^{real} &= \frac{(1-cc)\cdot n_{e^+}^{measure}-cc\cdot n_{e^-}^{measure}}{1-2cc} \\
n_{e^-}^{real} &= \frac{(1-cc)\cdot n_{e^-}^{measure}-cc\cdot n_{e^+}^{measure}}{1-2cc}
\end{aligned}
$$

# cc calculation
