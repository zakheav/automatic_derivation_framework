# automatic-differentiation-framework
一个支持控制流的自动求导框架
## 项目结构
* src/目录下是主要源代码实现，包括计算图，虚拟图等主要算法
* src/op_node是各种计算节点的实现
* unit_test/目录下是各个模块的单元测试

## 总览
自动求导是简化神经网络模型实现的一个重要机制，使用框架实现神经网络主要有以下几步：

1. 使用者通过把多个基本的运算节点组织成一张计算图;
2. 框架对这张计算图进行拓扑排序，并依次调用运算节点的运算函数op()从而实现前向传播；
3. 然后框架再对这张计算图进行逆拓扑排序，并依次调用运算节点的梯度计算函数grad_op()从而实现反向传播。

运算节点操作的数据均为张量Tensor

## 控制流
框架为了支持控制流，引入了虚拟图的概念。所谓控制流即两种虚拟图的节点，包括循环Loop和分支Branch。

用户使用虚拟图构造了一个神经网络的蓝图，然后只有在实际运行的时候某些节点才能决定是否走当前分支。通过运行虚拟图构造出实际的计算图，
然后在计算图上进行反向传播，从而训练模型。

由于引入了循环，会在逻辑上为虚拟图带来“环”，为了避免“环”的出现造成虚拟图无法进行拓扑排序，本框架将Loop循环节点看做一个子图，其中只包含一个循环。
相当于把原图按照循环划分成了多个子图，子图可以嵌套，然后把子图看成一个广义节点，则整体上的虚拟图将不出现循环，从而可以进行前向传播。