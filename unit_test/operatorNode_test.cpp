#include "../include/op_node/Add.h"
#include "../include/op_node/Bias.h"
#include "../include/op_node/SquareSum.h"
#include "../include/op_node/Minus.h"
#include "../include/op_node/AbsSum.h"
#include "../include/op_node/Mult.h"
#include "../include/op_node/Sigmoid.h"
#include "../include/op_node/Parameter.h"
#include "../include/op_node/Dropout.h"
#include "../include/ComputeGraph.h"
#include "../include/Tensor.h"
#include "../include/OperatorNode.h"
#include "../include/Optimizer.h"
#include <sys/time.h>
#include <vector>
#include <iostream>
using namespace std;

int main () {
    vector<int> shape1; shape1.push_back (2); shape1.push_back (2);
    vector<int> shape2; shape2.push_back (2); shape2.push_back (2);
    Tensor* t1 = new Tensor (shape1);
    Tensor* t2 = new Tensor (shape2);
    t1 -> init ();
    t2 -> init ();
    t1 -> display (); cout << endl;
    t2 -> display ();

    // 生成计算节点
    OperatorNode* p1 = new Parameter ("Parameter", "1", "0", t1);
    OperatorNode* p2 = new Parameter ("Parameter", "2", "0", t2);
    OperatorNode* add = new Add ("Add", "1", "0");
    OperatorNode* minus = new Minus ("Minus", "1", "0");
    OperatorNode* mult = new Mult ("Mult", "1", "0");
    OperatorNode* sigmoid = new Sigmoid ("Sigmoid", "1", "0");
    OperatorNode* square_sum = new SquareSum ("SquareSum", "1", "0");
    OperatorNode* abs_sum = new AbsSum ("AbsSum", "1", "0");
    OperatorNode* b = new Bias ("Bias", "1", "0");
    OperatorNode* dropout = new Dropout ("Dropout", "1", "0", t2, 0.5);

    // 构建计算图
    ComputeGraph cg;
    cg.add_node ("", p1);
    cg.add_node ("", p2);
    cg.add_node (p1 -> get_name (), add);
    cg.add_node (p2 -> get_name (), add);// 测试加法
    //cg.add_node (p1 -> get_name (), minus);
    //cg.add_node (p2 -> get_name (), minus);// 测试减法
    //cg.add_node (p1 -> get_name (), mult);
    //cg.add_node (p2 -> get_name (), mult);// 测试乘法
    //cg.add_node (p1 -> get_name (), sigmoid);// 测试sigmoid
    //cg.add_node (p1 -> get_name (), square_sum);// 测试SquareSum
    //cg.add_node (p1 -> get_name (), abs_sum);// 测试AbsSum
    //cg.add_node (p1 -> get_name (), b);
    //cg.add_node (p2 -> get_name (), b);// 测试偏置
    //cg.add_node (p1 -> get_name (), dropout);// 测试dropout
    // 构建转置图
    cg.build_reverse_graph ();
    // 初始化优化器，普通sgd
    Optimizer* optimizer = new Optimizer (0.1);
    cg.m_optimizer = optimizer;

    struct timeval start,end;  
    gettimeofday(&start, 0);  
    for (int i = 0; i < 1; ++i) {
        vector<Node*> result;
        cg.forward_propagation (result);
        cout << "fp result:................." << endl;
        ((OperatorNode*) result[0]) -> m_output -> display (); cout << endl;// 前向结果
        //((OperatorNode*) result[1]) -> m_output -> display ();
        cg.back_propagation ();
        cout << "bp result:................." << endl;
        p1 -> m_sum_grad -> display (); cout << endl;
        p2 -> m_sum_grad -> display ();// 反向结果
        cout << "new parameter:............." << endl;
        p1 -> m_output -> display (); cout << endl;
        p2 -> m_output -> display ();
        cg.release_tensor ();
    }
    gettimeofday(&end, 0);  
    long timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;  
    printf("time=%f\n",timeuse /1000000.0);  
}
