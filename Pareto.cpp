/*#include <vector>
#include <functional>
#include <iostream>*/

struct functions{
    std::function<double(cost_type&)> func1;
    std::function<double(Matrix,cost_type&)> func2;
};


typedef struct functions functions;


namespace Pareto{

    class Front{
        int values[2] = {};
        std::vector<int> indexes;
        PSO::Swarm* Cluster;

        public:
            Front(PSO::Swarm* ParticleSwarm){
                Cluster = ParticleSwarm;
                int num = ParticleSwarm->size[0];
                bool result = this->add_to_Pareto(ParticleSwarm->popln);
                // Can be made easier with Divide and Conquer
                //for(int ind=0; ind<num;ind++){
                //    if(this->dominate(Cluster->popln[ind],-1)){
                //        indexes.push_back(ind);
                //    }
                //}
            }

            bool add_to_Pareto(Matrix popln){
                std::cout<<"adding to pareto"<<std::endl;
                int count = 0;

                for(int member=0;member<popln.size();member++){
                    if(indexes.size()){
                        count += static_cast<int>(this->dominate(popln[member],member));
                    }
                    else{
                        if(this->init_pareto(popln[member],member)){
                            indexes.push_back(member);
                            count=1;
                        }
                    }
                }
                
                if(count>=1){
                    return 1;
                }
                std::cout<<"num pareto - "<<indexes.size()<<std::endl;
                return 0;
            }

            /*void add_functions(std::function<double(cost_type&)> func1, std::function<double(Matrix, cost_type&)> func2){
                obj_func.func1 = func1;
                obj_func.func2 = func2;
                return;
            }*/
        
        private:

            double function_value(std::vector<float>& mat){
                int x = mat[0];
                int y = mat[1];
                double result =  pow((x - M_PI),2) + pow((y - 2.72),2) + sin(3*x + 1.41) + sin(4*y - 1.73);
                return result;
            }

            double get_frac_value(Matrix popln, std::vector<float>& mat){
                int count = 0;
                for(cost_type& x : popln){
                    if (std::abs(function_value(x) - function_value(mat)) < 0.5){
                        count+=1;
                    }
                }

                    return count/popln.size();
            }


            bool dominate(cost_type& axes, int index){
                
                float value1 = function_value(axes);
                float value2 = get_frac_value(Cluster->popln, axes);
                
                std::cout<<"Values of functions"<<value1<<value2<<"indexes size"<<indexes.size()<<std::endl;
                for(int val : indexes){
                    float obj_1_thresh = function_value(Cluster->popln[val]);
                    float obj_2_thresh = get_frac_value(Cluster->popln, Cluster->popln[val]);

                    
                    std::cout<<"Values of thresholds"<<obj_1_thresh<<obj_2_thresh<<std::endl;
                    int ind = index;
                    if (value1 <= obj_1_thresh && obj_2_thresh-value2 <= 0.05 && obj_2_thresh-value2 >= 0){
                        indexes.push_back(ind);
                        return 1;
                    }
                    else if(value2 >= obj_2_thresh && obj_1_thresh-value1 <= -0.05 && obj_2_thresh-value2 <= 0){
                        indexes.push_back(ind);
                        return 1;
                    }
                }

                return 0;
            }

            bool init_pareto(cost_type& indv, int idx){
                bool* front = new bool;
                *front = 0;

                float value1 = function_value(indv);
                float value2 = get_frac_value(Cluster->popln, indv);

                for(int i=0; i<Cluster->size[0]; i++){
                    float obj_1_thresh = function_value(Cluster->popln[i]);
                    float obj_2_thresh = get_frac_value(Cluster->popln, Cluster->popln[i]);

                    if (value1 <= obj_1_thresh && obj_2_thresh-value2 >= 0){
                            *front = 1;
                        }
                    else if(value2 >= obj_2_thresh && obj_2_thresh-value2 <= 0){
                            *front = 1;
                    }
                }

                return *front;
            }

    };
}