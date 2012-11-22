#include "mcmc_span.h"
#include <cstdlib>
#include <ctime>

extern float* setdev_data(float* data, int dSize, int numElement);
extern float* setdev_dot(int numElement);
extern float* setdev_out(int numElement);
/**
 * \class compare function object for a set of patterns
 */
bool CompareSet::operator()(const pattern_t& set1,
        const pattern_t& set2) {
    return compare_set(set1, set2) < 0;
}
McmcSpan::~McmcSpan() {
    delete [] _m_raw_database;
}


void McmcSpan::set_data(const vector<vector<float> >& data) {
    _m_database = vector<vector<float> >(data);
    _m_N = _m_database.size();
    _m_label_num = _m_database.back().size();
    to_float(data, _m_raw_database);
}


void McmcSpan::run_alg(int mix_time, int k, ReduceFunc *reduce_func) {
    //A pattern is the same size as label number
    srand(time(NULL));
    pattern_t cur_pattern(_m_label_num);
    clear_pattern(cur_pattern);
    // A vector to store all the possible next state
    vector<pattern_t> next_state;
    int it_count =0;
    vector<pattern_t> history_stack;
    //loop until we find enough frequent pattern
    while(_m_max_pattern.size() < k) {
        it_count ++ ;
        //generate neighbor
        //1. go through all the label
        //2. If the label is not in the current pattern, and it's a frequent
        //patternpattern
        //   Add it to the current pattern as the next state
        //So that next state is basically neighbour frequent pattern
        for(int i=0; i < _m_label_num; i++) {
            //if the i-th label is zero
            if(!cur_pattern[i]) {
                cur_pattern[i] = 1;
                //If it's already in the
                if(_m_max_pattern.find(cur_pattern)!=_m_max_pattern.end() ||
                        (*reduce_func)(_m_raw_database,
                            cur_pattern,
                            _m_label_num,
                            _m_N ) > _m_N*_m_min_sup) {
                    next_state.push_back(cur_pattern);
                }
                //recover to origin, the next state has not selected yet
                cur_pattern[i] = 0;
            }
        }
        int surround_num = next_state.size();
        if(it_count > mix_time) {
            //Currrent pattern is a max pattern if it's surround states are not
            //frequent
            if(surround_num == 0) {
                _m_max_pattern.insert(cur_pattern);
                clear_pattern(cur_pattern);
                continue;
            }
        }
        //Random pick one as
        int ran_num = rand() % (surround_num+1);
        //For some certain probability, revert to previous state
        if(ran_num == surround_num) {
            cur_pattern = history_stack.back();
            history_stack.pop_back();
        } else {
            history_stack.push_back(cur_pattern);
            cur_pattern = next_state[ran_num];
        }
    }
}

void McmcSpan::init_gpu() {
    int numElement = _m_N;
    int numAttribute = _m_label_num;
    int dSize = numElement*numAttribute;
    int numThread = 512;
    float result = 0;
    float* data = _m_raw_database;
    dev_data = setdev_data(data, dSize, numElement);
    dev_dot = setdev_dot(numElement);
    dev_out = setdev_out(numElement);

}

void McmcSpan::print_max_pat() {
    pattern_db_t::iterator iter;
    for(iter = _m_max_pattern.begin();
            iter!= _m_max_pattern.end();
            iter++) {
        for(int i = 0; i<iter->size(); i++) {
            if((*iter)[i]!=0) {
                cout << i << " ";
            }
        }
        cout << endl;
    }
}

