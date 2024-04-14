/*#include <iostream>
#include <vector>
#include <omp.h>
#include <functional>*/
#include "datatypes/dtype.cpp"
//#include "PSO.cpp"
//#include "Pareto.cpp"
#define MAX_DEPTH 4
#define NUM_THREADS 3
#define MAX_GEN 5

//typedef std::vector<float> cost_type

/*struct PopSpace_return{
    bool status;
    Pareto::Front* pareto_front;
};

typedef struct PopSpace_return PopSpace_return;*/

/*bool Add_to_Pareto(Matrix& swarm, Pareto::Front& pareto_front){
    std::vector<short> result(swarm.size());

    for(int idx=0; idx<swarm.size(); idx++){
        result.push_back(static_cast<short>(pareto_front.dominate(swarm[idx], idx)));
    }

    if (std::reduce(result.begin(), result.end()) > 0){
        return 1;
    }
    return 0;
}*/


pos_value* SelectElite(PSO::Swarm& family){
    //Getting the cost value
    std::cout<<std::endl<<"Entered Elite Selection"<<std::endl;
    pos_value* per_indv = new pos_value;

    for(int i=0;i<family.size[0];i++){
        //std::cout<<"Member "<<i<<" Checked"<<std::endl;
        per_indv->cost.push_back(family.get_cost_value(family.popln[i]));
    }

    //Sorting the values and choosing the top 1/7th
    float top = family.size[0]/7;
    if(family.size[0] == 10){
        std::cout<<"####################################################################"<<std::endl;
    }
    std::cout<<"Family size - "<<family.size[0]<<std::endl;
    std::cout<<"TOP - "<<top<<std::endl;
    per_indv->elite = Utils::get_top_k(top,per_indv->cost);
    
    std::cout<<std::endl<<"Elite Selection Completed"<<std::endl;
    return per_indv;
    
}

bool PopSpaceAlg(PSO::Swarm& ParticleSwarm, Pareto::Front& pareto_front, int limit){
    short num = 0;
    int pop_size = ParticleSwarm.size[0];
    short patience = 0;
    //The loop with condition on num_repeats(limit) and pareto threshold
    while(num<limit){
        std::cout<<"Entered PopSpaceAlg"<<std::endl;

        //Add the velocity to the position of the particles in space

        #pragma omp parallel for
        for(int i=0;i<pop_size;i++){
            std::cout<<"Particle "<<i<<" moved"<<std::endl;
            ParticleSwarm.popln[i][0] += ParticleSwarm.velocity[i][0];
            ParticleSwarm.popln[i][1] += ParticleSwarm.velocity[i][1];
        }

        //Adding to Pareto Front
        bool pareto_patience = pareto_front.add_to_Pareto(ParticleSwarm.popln);
        patience += (pareto_patience==0) ? 1:0;

        if(patience == MAX_GEN){
            return 0;
        }

        std::cout<<"Particles have taken a step"<<std::endl;

        //initialise beliefs
        ParticleSwarm.init_pbelief();

        //Select the elite from the space
        pos_value* elite_indv = SelectElite(ParticleSwarm);

        //Mutate and Crossover done over the elite and the rest of the swarm
        std::cout<<elite_indv->elite.size()<<std::endl;
        PSO::mutate_crossover(ParticleSwarm, pop_size, elite_indv, elite_indv->elite.size());

        //Accceptence and update step -> Cultural Alg Step
        ParticleSwarm.accept_and_update(elite_indv);

        //Influence Step -> CA Step
        std::cout<<"Population being influenced"<<std::endl;
        #pragma omp parallel for
        for(int i=0; i<pop_size;i++){
            if(PSO::isthere(i,elite_indv->elite)){
                std::cout<<"no"<<std::endl;
                continue;
            }
            else{
                std::cout<<"yes"<<std::endl;
                ParticleSwarm.influence(elite_indv,ParticleSwarm.velocity[i],ParticleSwarm.personal_belief[i]);
            }
        }
        std::cout<<"Population influenced"<<std::endl;

        num+=1;
    }

    std::cout<<"Finished one cycle of PopScale"<<std::endl;
    return 1;
}


void SearchInSpace(PSO::Swarm& ParticleSwarm, Pareto::Front& pareto_front, short depth, int maxrepeat){
    std::cout<<"Entered SearchInSpace"<<std::endl;
    bool result = PopSpaceAlg(ParticleSwarm, pareto_front, maxrepeat);
    if (!(result)){
        return;
    }
    std::cout<<"Finished the FIRST PopSpace"<<std::endl;
    
    //return;
    std::vector<PSO::Swarm> splitted;

    if(result && depth>0)
    {
        //omp_set_num_threads(2);
        splitted = ParticleSwarm.split();
        //depth-=1;

        #pragma omp task shared(splitted,depth)
        {
            SearchInSpace(splitted[0], pareto_front, depth-1, maxrepeat);
        }
        
        #pragma omp task shared(splitted,depth)
        {
            SearchInSpace(splitted[1], pareto_front, depth-1, maxrepeat);
        }

    }

    else if(result)
    {
        //depth+=1;
        std::cout<<"SUCESSS##########################################SUCESSS"<<std::endl;
        SearchInSpace(ParticleSwarm, pareto_front, depth-1, maxrepeat);
    }

    else
    {
        return;
    }
    
}


int main(){
    omp_set_num_threads(NUM_THREADS);
    int num = 20;
    PSO::Swarm ParticleSwarm(num);

    //std::function<double(cost_type&)> func1 = function_value;
    //std::function<double(Matrix, cost_type&)> func2 = get_frac_value;

    Pareto::Front pareto_front(&ParticleSwarm);
    //pareto_front.add_functions(func1, func2);

    SearchInSpace(ParticleSwarm,pareto_front,1,10);
    return 0;
}
