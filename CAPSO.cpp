/*#include <iostream>
#include<vector>
#include <cmath> 
#include <math.h>
#include <omp.h>
#include <ctime>
#include <map>*/

namespace CS{
    
    typedef std::vector<std::vector<float>> Matrix;

    class CA{
        
        public:
            std::map<size_t,std::vector<float>> belief;//(num, std::vector<float>(2,0.0));
            Matrix personal_belief;


            //Belief Generator (To be changed to a NN)
            void add_belief(std::vector<size_t>& indv, std::string dist){
                std::cout<<"elite index - "<<indv[0]<<std::endl;
                for(int i=0; i<indv.size();i++){
                    if(dist=="normal"){
                        //float mean = (rand() %(4));
                        //float std = (rand() %(4));
                        //belief.insert({indv[i],{mean, std}});
                        belief[indv[i]] = personal_belief[i];
                    }
                    else{
                        //float alpha = (rand() %(4));
                        //float beta = (rand() %(4));
                        //belief.insert({indv[i],{alpha, beta}});
                        belief[indv[i]] = personal_belief[i];//{alpha,beta};
                    }
                }
                /*#pragma omp parallel for shared(belief)
                for(int i=0; i<num; i++){
                  float mean = (rand() %(4));
                  float std = 1;
                  belief[i] = {mean,std};
                }*/
                
                return;
            }
            
            void accept_and_update(pos_value* group){
                std::cout<<"Entered Accept and Update"<<std::endl;
                this->add_belief(group->elite,"normal");
                std::cout<<"Added Belief Space"<<std::endl;
            }

            void influence(pos_value* elite_group, cost_type& velocity, cost_type& indv_KS){

                //Randomly opting an elite for influence
                size_t num_elite = elite_group->elite.size();
                std::cout<<"KS - "<<num_elite<<std::endl;
                size_t rand_elite = rand()%(num_elite);

                //retrieving the target Knowledge source
                std::vector<float> target_KS = belief[elite_group->elite[rand_elite]];
                std::cout<<"Target KS"<<target_KS.size()<<std::endl;
                std::cout<<"Indv KS"<<indv_KS.size()<<std::endl;
                //influence step
                std::cout<<"Wasser"<<std::endl;
                double work = Utils::Wasserstein(target_KS, indv_KS);
                std::cout<<"Wassersteins distance calculated"<<std::endl;
                float weight = std::tanh(work);

                //Perform the weighted average
                indv_KS[0] = (1-weight)*target_KS[0] + weight*(indv_KS[0]);
                indv_KS[1] = (1-weight)*target_KS[1] + weight*(indv_KS[1]);

                return;

            }
              
    };


}

/*using namespace CS;

int main(){
        CA family;
        family.initialise(2,2);
        for(int i=0;i<family.size[0];i++){
            std::cout<<"Beliefs - "<<family.beliefs[i][0]<<" "<<family.beliefs[i][1]<<" ";
            for(int j=0;j<family.size[1];j++){
                std::cout<<family.popln[i][j]<<" ";
                
            }
            std::cout<<std::endl;
        }
        return 0;
    }*/
