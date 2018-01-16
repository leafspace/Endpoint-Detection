
typedef struct {
    float x;  // 系统的状态量
    float A;  // x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
    float H;  // z(n)=H*x(n)+w(n),w(n)~N(0,r)
    float q;  // 预测过程噪声协方差
    float r;  // 测量过程噪声协方差
    float p;  // 估计误差协方差
    float gain;//卡尔曼增益
}KalmanStructTypedef;

/**
 *@function: - 卡尔曼滤波器初始化
 *@kalmanFilter：卡尔曼滤波器结构体
 *@init_x：待测量的初始值
 *@init_p：后验状态估计值误差的方差的初始值
*/
void kalmanFilter_init(KalmanStructTypedef *kalmanFilter, float init_x, float init_p,float predict_q,float newMeasured_q)
{
    kalmanFilter->x = init_x;//待测量的初始值，如有中值一般设成中值
    kalmanFilter->p = init_p;//后验状态估计值误差的方差的初始值（不要为0问题不大）
    kalmanFilter->A = 1;
    kalmanFilter->H = 1;
    kalmanFilter->q = predict_q;//预测（过程）噪声方差 影响收敛速率，可以根据实际需求给出
    kalmanFilter->r = newMeasured_q;//测量（观测）噪声方差 可以通过实验手段获得
}

/**
 *@function: - 卡尔曼滤波器
 *@kalmanFilter:卡尔曼结构体
 *@newMeasured；测量值
 *返回滤波后的值
 */
float kalmanFilter_filter(KaslmanStructTypedef *kalmanFilter, float newMeasured)
{
    /* Predict */
    kalmanFilter->x = kalmanFilter->A * kalmanFilter->x;//%x的先验估计由上一个时间点的后验估计值和输入信息给出
    kalmanFilter->p = kalmanFilter->A * kalmanFilter->A * kalmanFilter->p + kalmanFilter->q;  /*计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q */
 
    /* Correct */
    kalmanFilter->gain = kalmanFilter->p * kalmanFilter->H / (kalmanFilter->p * kalmanFilter->H * kalmanFilter->H + kalmanFilter->r);
    kalmanFilter->x = kalmanFilter->x + kalmanFilter->gain * (newMeasured - kalmanFilter->H * kalmanFilter->x);//利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出
    kalmanFilter->p = (1 - kalmanFilter->gain * kalmanFilter->H) * kalmanFilter->p;//%计算后验均方差
 
    return kalmanFilter->x;
}

