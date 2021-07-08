#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void Print(string filename, vector< vector<uint32_t> > features, vector< uint32_t > labels) {
    ofstream fout(filename + ".dat");
    fout << features.size() << endl;
    fout << features[0].size() << endl;
    for (auto i=0; i<features.size(); ++i) {
        for (auto j=0; j<features[i].size(); ++j) {
            fout << features[i][j] << ",";
        }
        fout << endl;
    }
    for (auto i=0; i<labels.size(); ++i) {
        fout << labels[i] << endl;
    }
}

void Print(string filename, vector< vector<uint32_t> > features, vector< uint32_t > labels, vector< uint32_t > tags) {
    ofstream fout(filename + ".dat");
    fout << features.size() << endl;
    fout << features[0].size() << endl;
    for (auto i=0; i<features.size(); ++i) {
        for (auto j=0; j<features[i].size(); ++j) {
            fout << features[i][j] << ",";
        }
        fout << endl;
    }
    for (auto i=0; i<labels.size(); ++i) {
        fout << labels[i] << ' ' << tags[i] << endl;
    }
}

void Print(string filename, vector< vector<uint32_t> > features) {
    ofstream fout(filename + ".dat");
    fout << features.size() << endl;
    fout << features[0].size() << endl;
    for (auto i=0; i<features.size(); ++i) {
        for (auto j=0; j<features[i].size(); ++j) {
            fout << features[i][j] << ",";
        }
        fout << endl;
    }
}

void Print(string filename, vector< vector<uint64_t> > features, vector< uint64_t > labels) {
    ofstream fout(filename + ".dat");
    fout << features.size() << endl;
    fout << features[0].size() << endl;
    for (auto i=0; i<features.size(); ++i) {
        for (auto j=0; j<features[i].size(); ++j) {
            fout << features[i][j] << ",";
        }
        fout << endl;
    }
    for (auto i=0; i<labels.size(); ++i) {
        fout << labels[i] << endl;
    }
}

void Print(string filename, vector< vector<uint64_t> > features, vector< uint64_t > labels, vector< uint64_t > tags) {
    ofstream fout(filename + ".dat");
    fout << features.size() << endl;
    fout << features[0].size() << endl;
    for (auto i=0; i<features.size(); ++i) {
        for (auto j=0; j<features[i].size(); ++j) {
            fout << features[i][j] << ",";
        }
        fout << endl;
    }
    for (auto i=0; i<labels.size(); ++i) {
        fout << labels[i] << ' ' << tags[i] << endl;
    }
}

int main() {
    // generate one-to-one join
    // table 1 ID - fea1 - fea2
    // table 2 val1ID - lab
    // y = 24 * x0 + 27 * x1 + 97

    srand(97224127);

    uint32_t MOD = 1024;
    uint32_t Ntuples = 1000;
    uint32_t Nattrs = 3;
    vector< vector<uint32_t> > features (Ntuples);
    vector< uint32_t > labels (Ntuples);

    uint32_t Nreal = 100;
    vector< uint32_t > tags (Ntuples);

    for (uint32_t i=0; i<Ntuples; ++i) {
        features[i].resize(Nattrs);
        for (uint32_t j=0; j<Nattrs-1; ++j) {
            features[i][j] = rand() % MOD;
        }
        features[i][Nattrs - 1] = 1;
        if (i < Nreal) {
            labels[i] = features[i][0] * 24 + features[i][1] * 27 + features[i][2] * 97;
            tags[i] = 1;
        } else {
            labels[i] = rand() % MOD;
            tags[i] = 0;
        }
    }

    Print("traindata_plain_withtag", features, labels, tags);

    vector< vector<uint64_t> > shareFeatures1(Ntuples), shareFeatures2(Ntuples);
    vector<uint64_t> shareLabels1(Ntuples), shareLabels2(Ntuples);
    vector<uint64_t> shareTags1(Ntuples), shareTags2(Ntuples);

    for (auto i=0; i<Ntuples; ++i) {
        shareFeatures1[i].resize(Nattrs);
        shareFeatures2[i].resize(Nattrs);
        for (auto j=0; j<Nattrs; ++j) {
            shareFeatures1[i][j] = ((rand() * 1ULL) << 32) + rand();
            shareFeatures2[i][j] = features[i][j] - shareFeatures1[i][j];
        }
        shareLabels1[i] = ((rand() * 1ULL) << 32) + rand();
        shareLabels2[i] = labels[i] - shareLabels1[i];
        shareTags1[i] = ((rand() * 1ULL) << 32) + rand();
        shareTags2[i] = tags[i] - shareTags1[i];
    }

    Print("traindata_share1_withtag", shareFeatures1, shareLabels1, shareTags1);
    Print("traindata_share2_withtag", shareFeatures2, shareLabels2, shareTags2);

    // vector< vector<uint32_t> > table1(Ntuples), table2(Ntuples);
    // for (auto i=0; i<Ntuples; ++i) {
    //     table1[i].resize(3);
    //     table2[i].resize(2);
    //     table1[i][0] = i; table1[i][1] = features[i][0]; table1[i][2] = features[i][1];
    //     table2[i][0] = i; table2[i][1] = labels[i];
    // }

    // Print("table1", table1);
    // Print("table2", table2);

    return 0;
}
