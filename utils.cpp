#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

//typedef std::vector<float> cost_type;
//typedef std::vector<std::vector<float>> Matrix;


namespace Utils{
    std::vector<size_t> get_top_k(int k, cost_type& cost){
        std::vector<size_t> elite;

        std::vector<size_t> idx(cost.size());
        iota(idx.begin(), idx.end(), 0);
        std::cout<<"Created index vector"<<std::endl;

        //for(int i=0; i<cost.size(); i++){
        //    std::cout<<idx[i]<<std::endl;
        //}

        std::sort(idx.begin(), idx.end(), std::greater<size_t>()); //[&cost](size_t i, size_t j){cost[i]<cost[j];});

        std::cout<<"Array Sorted"<<std::endl;

        //for(int i=0; i<cost.size(); i++){
        //    std::cout<<idx[i]<<std::endl;
        //}
        for(int i=0 ; i<k ; i++){
            
            elite.push_back(idx[i]);
        }
        //std::cout<<elite.size()<<std::endl;
        std::cout<<"Elites pushed"<<std::endl;

        return elite;
    }

    void Mutate(cost_type& one, int scale){
        std::cout<<"Mutation Selected"<<std::endl;
        int lb = -1*scale;
        float random_x = rand()/RAND_MAX*(scale-lb+1) + lb;
        float random_y = rand()/RAND_MAX*(scale-lb+1) + lb;
        one[0] += random_x;
        one[1] += random_y;
        return;
    }

    void Crossover(cost_type& one, cost_type& two, float factor){
        std::cout<<"Crossover Selected"<<std::endl;
        std::cout<<one[0]<<one[1]<<std::endl;
        std::cout<<two[0]<<two[1]<<std::endl;
        one[0] = factor*one[0] + (1-factor)*two[0];
        one[1] = factor*one[1] + (1-factor)*two[1];
        return;

    }
    double Wasserstein(std::vector<float>& dist1, std::vector<float>& dist2){

        std::cout<<"location"<<dist1.size()<<dist2.size()<<std::endl;
        double location = std::pow(dist1[0] - dist2[0],2);
        
        double size = pow(dist1[0] - dist2[0],2);
        std::cout<<"size"<<size<<std::endl;
        double work = pow(location+size, 0.5);
        std::cout<<"Wasser calculated"<<location<<size<<work<<std::endl;

        return work;
    }


    /*class Functions{

        public:

            static auto function_value(std::vector<float>& mat){
                int x = mat[0];
                int y = mat[1];
                return pow((x - M_PI),2) + pow((y - 2.72),2) + sin(3*x + 1.41) + sin(4*y - 1.73);
            }

            static auto neighbour_density(){
                return 1/2;
            }
    };*/
};
