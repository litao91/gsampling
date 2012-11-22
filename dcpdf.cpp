#include "dcpdf.h"
void updatePDF(vector<double>& pdf,  const double& pr){
    if(pdf.empty()) {
        pdf.push_back(1.0);
    }

    int n = pdf.size()+1;
    vector<double> tmp;
    tmp.resize(n);

    tmp[0] = (1-pr) * pdf[0];

    for(int i=1; i<n; i++) {
        if(i+1<n)
            tmp[i] = pdf[i-1] * pr + pdf[i] * (1-pr);
        else
            tmp[i] = pdf[i-1] * pr;
    }
    pdf = tmp;
}

void dpPDF(vector<double>& pdf,  const vector<double> & prList) {
    int n = prList.size();

    for(int i=0; i<n; ++i)
        updatePDF(pdf, prList[i]);
}

double cdf(vector<double>& pdf, int minsup) {
    double result = 0;
    int i = -1;
    for(vector<double>::iterator ptr=pdf.begin();
        ptr!=pdf.end(); ++ptr){
        if(i >= minsup-1){
            return result;
        }
        result += (*ptr);
        i = i + 1;
    }
    return result;
}

void divAndConq1(vector<double>& pdf,
        int left,
        int right,
        const vector<double>& v) {
    int n = right - left + 1;

    if(n <= 2){
        vector<double> tmp = vector<double>(v.begin()+left, v.begin()+right+1);
        dpPDF(pdf, tmp);
        return ;
    }

    int mid = (left+right) / 2;

    vector<double> p1, p2;

    divAndConq1(p1, left, mid, v);
    divAndConq1(p2, mid+1, right, v);

    convol(pdf, p1, p2);
}


void dcPDF1(vector<double>& pdf, const vector<double>& v){
    divAndConq1(pdf, 0, v.size()-1, v);
}


/*
int main(){
    double pdf[]={0.5,0.5,0.5,0.5,0.25,0.75};
    // The input array: [Pr(p \in g1), .... Pr(p\ingn)]
    vector<double> pdf1 (pdf,pdf+sizeof(pdf)/sizeof(double));
    vector<double> pdf2;
    initTab(pdf1.size()*8);
    vector<double> pdf3;
    // pdf3 is the output of dpPDF,
    // .ouput = [pr(#p = 0], .... Pr(#p = n)]
    dpPDF(pdf3,pdf1);
    for(vector<double>::iterator ptr=pdf3.begin();ptr!=pdf3.end();++ptr) {
        cout<<(*ptr)<<" ";
    }
    cout<<endl;
    // Calculate sum of variable based on dp
    dcPDF1(pdf2,pdf1);
    for(vector<double>::iterator ptr=pdf2.begin();ptr!=pdf2.end();++ptr){
        cout<<(*ptr)<<" ";
    }
    cout<<endl;
    return 0;
}
*/
