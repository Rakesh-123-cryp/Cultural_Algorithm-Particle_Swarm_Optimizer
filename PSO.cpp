//#include <iostream>
//#include <vector>
//#include <omp.h>
//#include <math.h>
//#include <ctime>
//#include "utils.cpp"
//#include "CAPSO.cpp"
#define SPACE_DIM 2
#define SCALE 2
#define FACTOR 0.2f


//typedef std::vector<std::vector<float>> Matrix;
//typedef std::vector<float> cost_type;

/*typedef struct elite_return{
    pos_value* elite_indv;
    vector<int> elite_indices;
} elite_return;

struct pos_value{
    cost_type cost;
    vector<size_t> elite;
};

typedef struct pos_value pos_value;
typedef struct elite_return elite_return;*/


namespace PSO{
    //#define NUM_THREADS 2

    class Swarm:public CS::CA
    {
        public:
          Matrix popln;
          Matrix velocity;
          int size[2] = {0,SPACE_DIM};
          
          Swarm(){
            ;
          }

          Swarm(int num){
              size[0] = num;
              initialise(num,size[0]);
          }
          
          // function to get the value of the function space

          float get_cost_value(std::vector<float>& mat){
                int x = mat[0];
                int y = mat[1];
                return this->cost_func(x,y);
            }


          std::vector<Swarm> split(){
              short length = size[0];
              int size1 = length/2;
              Swarm FirstHalf(size1);
              Swarm SecondHalf(length-size1);
              
              FirstHalf.size[1] = SecondHalf.size[1] = this->size[1];

              for(int i=0;i<length/2;i++){
                  FirstHalf.popln.push_back(popln[i]);
                  SecondHalf.popln.push_back(popln[i+length/2]);
              }
              if (size[0]%2!=0){
                  SecondHalf.popln.push_back(popln[length-1]);
              }

              std::vector<Swarm> split_array(2);
              
              split_array[0] = FirstHalf;
              split_array[1] = SecondHalf;
              return split_array;
          }
          
          void init_pbelief(){

                for(int idx=0;idx<this->size[0];idx++){
                    float mean = (rand() %(4));
                    float std = (rand() %(4));
                    personal_belief.push_back({mean,std});
                }
                std::cout<<"Initialised Belief"<<std::endl;
                return;
                
            }
        private:
            
            // Function to initialize the population
            void initialise(int n, int m){
                size[0] = n;
                size[1] = m;
                popln = generate_matrix(size[0],size[1],0,10);
                velocity = generate_matrix(size[0],size[1],1,5);
                return;
            }
            
            // Random generation for the initial population
            Matrix generate_matrix(int& n, int& m, int lb, int ub){
                Matrix matrix(n, std::vector<float>(m,0.0));
                //omp_set_num_threads(3);
                
                //Getting Time of Execution
                time_t start, end;
                std::time(&start);
                
                #pragma omp parallel for
                for(int i=0;i<n;i++){
                    #pragma omp parallel for
                    for(int j=0;j<m;j++){
                        matrix[i][j] = (rand() %(ub-lb+1)) + lb;
                    }
                }
                //Terminate recording time
                std::time(&end);
                double time_lapsed = double(end-start);
                
                return matrix;
            }
            
            // The function space
            int cost_func(int& x, int&y){return pow((x - M_PI),2) + pow((y - 2.72),2) + sin(3*x + 1.41) + sin(4*y - 1.73);}
            
    };

    bool isthere(size_t index, std::vector<size_t> elite){
        for(auto i : elite){
            if(i==index){
                return 1;
            }
        }
        return 0;
    }

    void mutate_crossover(Swarm& family, int size, pos_value* elite_group, size_t k){ //Return Swarm
        std::cout<<"Mutation and Crossover Started"<<std::endl;
        for(int i=0;i<size;i++){

            if(isthere(i,elite_group->elite)){
                continue;
            }

            float rand0 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
            float rand1 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
            float rand2 = ((static_cast<float>(rand())/static_cast<float>(RAND_MAX))*(rand1 - 1)) + rand1;
            float rand3 = ((static_cast<float>(rand())/static_cast<float>(RAND_MAX))*(rand2 - 1)) + rand2;

            std::cout<<"Random floats declared"<<std::endl;

            std::vector<size_t> elite_ind = elite_group->elite;
            if (rand0<rand1){
                
                int index = rand()%(k);
                std::cout<<"Mutate and Crossover "<<" - "<<k<<std::endl;
                std::cout<<"elite index selected "<<" - "<<index<<std::endl;
                Utils::Crossover(family.velocity[i],family.velocity[elite_ind[index]],FACTOR);
                Utils::Mutate(family.velocity[i],SCALE);
                //Mutate and Crossover
                
            }

            else if (rand0<rand2){
                
                int index = rand()%(k);
                std::cout<<"Crossover"<<std::endl;
                Utils::Crossover(family.velocity[i],family.velocity[elite_ind[index]],FACTOR);
                //Crossover only
            }

            else if (rand0<rand3){
                std::cout<<"Mutate"<<std::endl;
                Utils::Mutate(family.velocity[i],SCALE);
                //Mutate only
            }

            else{
                //None
                continue;
            }


        }
        std::cout<<"Mutation and Crossover Completed"<<std::endl;

        return;
    }

    

};

//using namespace PSO;

/*int main(int argc, char* argv[]){
    std::cout<<(argv[0])<<std::endl;
    PSO::Swarm family(10);
    
    for(int i=0;i<family.size[0];i++){
            for(int j=0;j<family.sizje[1];j++){
                std::cout<<family.popln[i][j]<<" ";
                
            }
            std::cout<<std::endl;
        }
        
    return 0;
}*/
