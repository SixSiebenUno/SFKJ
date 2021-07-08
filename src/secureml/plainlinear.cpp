#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <time.h>

#include <Eigen/Dense>

#include <common.h>
#define bigN N

using namespace std;
using namespace Eigen;

typedef unsigned long int uint64;
typedef Matrix<double,Dynamic,Dynamic> Mat;
typedef Matrix<double,Dynamic,Dynamic> MatD;

void load_test_data(MatD& data, MatD& label){
	// ifstream infile("../../data/iris.csv" );
	ifstream infile("../data/oulad.dat" );
	int i=0;
    string s;
    // getline(infile,s);
    // getline(infile,s);
    // getline(infile,s);
	//cout<<"load testing data.......\n";
	while(infile){
		if (!getline(infile,s)) 
			break;
		cout << s << endl;
		istringstream ss(s);
		int temp;
		char c;

		ss >> label(i);
		ss >> c;
        for (int j=0; j<D-1; ++j) {
            ss>>data(i,j);
			ss>>c;
        }
        data(i,D-1) = 1;
        data.row(i) /= 32;
		// data.row(i).normalize();

        // string name;
        // ss>>name;
        // if (name == "setosa") label(i) = 0;
        // else if (name == "versicolor") label(i) = 1;
        // else if (name == "virginica") label(i) = 2;
        ++i;
        
	}
	data.conservativeResize(i, D);
	label.conservativeResize(i,1);
	infile.close();
	cout << data.row(0) << ' ' << label.row(0) << endl;
	return;
}

void next_batch(Mat &batch, uint32_t &start, Mat A) {
    for (auto i=0; i<B; ++i) {
        batch.row(i) = A.row(start);
        start = (start + 1) % A.rows();
    }
}

void next_batch_perm(Mat &batch, uint32_t &start, Mat A, vector<int32_t> perm) {
    for (auto i=0; i<B; ++i) {
        if (perm[start] == -1) {
            batch.row(i).setZero();
        } else {
            batch.row(i) = A.row(perm[start]);
        }
        start = (start + 1) % perm.size();
    }
}

void linear_function(Mat x, Mat w, Mat &fx) {
    fx = x * w;
}

void logistic_function(Mat x, Mat w, Mat &fx) {
    fx = x * w;
    for (auto i=0; i<fx.rows(); ++i) {
        fx(i) = 1 / (1 + exp(fx(i)));
    }
}

double test_model(Mat w, Mat x, Mat y) {
    Mat y_ = x * w;
    uint32_t count = 0, all = y.rows();
    for (auto i=0; i<all; ++i) {
        if (round(y_(i)) == y(i)) {
            ++count;
        }
    }
    return 1.0 * count / all;
}

int main(int argc, char** argv) {
    // load data
    Mat train_data(N,D), train_label(N,1);
    Mat test_data(testN,D), test_label(testN,1);
    load_test_data(train_data, train_label);
    load_test_data(test_data, test_label);
    srand(time(0));

    for (auto i=0; i<train_data.rows(); ++i) {
        // train_data.row(i) /= train_data.row(i).maxCoeff();
        // train_data.row(i).normalize();
    }
    for (auto i=0; i<test_data.rows(); ++i) {
        // test_data.row(i) /= test_data.row(i).maxCoeff();
        // test_data.row(i).normalize();
    }
    
    // training phase
    Mat W(D, 1);
    for (auto i=0; i<D; ++i) {W(i) = 0;}
    // W.normalize();
    W /= B;
    Mat x_batch(B,D), tx_batch, y_batch(B,1);
    Mat fx_batch(B, 1), delta(D, 1);

    uint32_t batchcheck = 10;
    uint32_t start_data = 0, start_label = 0;
    double lr = 0.01 / B; 
    //0.0024

// oulad coefficient
// # -0.00427, 0.00276, -0.168, 0.0206, 0, 0.0127, -0.373

    vector<int32_t> perm(N);
    for (auto i=0; i<N; ++i) {
        perm[i] = i;
    }
    random_shuffle(perm.begin(), perm.end());

    vector<uint32_t> iter, iter_puri, iter_dummy;
    vector<double> acc_baseline = {0.4, 0.6, 0.8, 0.9, 1.0};
    uint32_t acc_id = 0;

    std::random_device rd{};
    std::mt19937 gen{127}; // 97127
    std::normal_distribution<> d{0, 1};
    double bias = 354.79 * 2.24;
    Mat dp(D, 1);

    vector<double> accuracy;
    for (auto i=0; i<5000; ++i) {
        next_batch_perm(x_batch, start_data, train_data, perm);
        next_batch_perm(y_batch, start_label, train_label, perm);
        tx_batch = x_batch.transpose();
        linear_function(x_batch, W, fx_batch);
        delta = tx_batch * (fx_batch - y_batch);
        // cout << (fx_batch - y_batch) << endl;
        // delta = delta / delta.norm();
        for (auto j=0; j<D; ++j) {
            dp(j, 0) = d(gen) * bias;
        }
        delta = delta + dp;
        W = W - delta * lr;
        if (i % batchcheck == 0) {
            accuracy.push_back(test_model(W, test_data, test_label));
            cout << i << ' ' << test_model(W, test_data, test_label) << endl;
            // cout << "W = " << W << endl << endl;
        }
    }
    cout << bias << endl;
    cout << "accuracy = " << test_model(W, test_data, test_label) << endl;

    // MatD newW(D, 1);
    // newW << -0.00427, 0.00276, -0.168, 0.0206, 0, 0.0127, -0.373;
    // newW *= 32;
    // cout << "accuracy = " << test_model(newW, test_data, test_label) << endl;


    return 0;

    vector<double> acc_dummy, acc_puri;
    vector<int32_t> perm_dummy(bigN), perm_puri(bigN);
    for (auto i=0; i<bigN; ++i) {
        perm_dummy[i] = (i < N) ? i : -1;
        perm_puri[i] = i % N;
    }
    random_shuffle(perm_dummy.begin(), perm_dummy.end());
    // random_shuffle(perm_puri.begin(), perm_dummy.end());

    W.setZero();
    start_data = start_label = 0;
    acc_id = 0;
    for (auto i=0; i<IT; ++i) {
        next_batch_perm(x_batch, start_data, train_data, perm_dummy);
        next_batch_perm(y_batch, start_label, train_label, perm_dummy);
        tx_batch = x_batch.transpose();
        linear_function(x_batch, W, fx_batch);
        delta = tx_batch * (fx_batch - y_batch);
        // delta = delta / delta.norm();
        W = W - delta * lr;
        if (i % batchcheck == 0) {
            acc_dummy.push_back(test_model(W, test_data, test_label));
            cout << i << ' ' << test_model(W, test_data, test_label) << endl;
        }
    }

    W.setZero();
    start_data = start_label = 0;
    acc_id = 0;
    for (auto i=0; i<IT; ++i) {
        next_batch_perm(x_batch, start_data, train_data, perm_puri);
        next_batch_perm(y_batch, start_label, train_label, perm_puri);
        tx_batch = x_batch.transpose();
        linear_function(x_batch, W, fx_batch);
        delta = tx_batch * (fx_batch - y_batch);
        // delta = delta / delta.norm();
        W = W - delta * lr;
        if (i % batchcheck == 0) {
            acc_puri.push_back(test_model(W, test_data, test_label));
            cout << i << ' ' << test_model(W, test_data, test_label) << endl;
        }
    }  

    // output phase
    freopen("train_log.log", "w", stdout);
    for (auto i=1; i<acc_puri.size(); ++i) {
        cout << i * batchcheck << ' ' << accuracy[i] << ' ' << acc_dummy[i] << ' ' << acc_puri[i] << endl;
    }
}
