/*
Heuristik för Rollbesättningsproblemet. 

* Authors: Farhan Syed & Abhinav Sasikumar
* Year: 2023
*/

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;
vector<vector<int>> role_constraints; 
vector<vector<int>> scene_constraints;
vector<vector<int>> actorsVector;
int n, s, k;

/*
    Checks whether two roles are in the same scene. 
    Return true if both are in same scene. Otherwise return false.  

*/
bool checkRolesInSameScene(int role1, int role2){
    for(int i=1;i<scene_constraints.size();i++){ // Go through all scenes
        bool containsRole1 = false;
        bool containsRole2 = false;

        for (int j = 0; j < scene_constraints.at(i).size(); j++) {
            if (scene_constraints.at(i).at(j) == role1) { // contains the first role
                containsRole1 = true;
            }
            if (scene_constraints.at(i).at(j) == role2) { // contains the second role
                containsRole2 = true;
            }
        }
        if (containsRole1 && containsRole2) { // Both are in the same role
            return true;
        }
    }
    return false;
}

/*
    Checks whether an actor has a clashing role. 
    For all actors, this entails a role that plays in a scene that the actor already has a role in. 
    For actors 1 and 2, this also entails a role that plays in a scene that the other actor is playing in.    
    Return true if the role is clashing. Otherwise return false.  

*/
bool checkActorHasClashingRole(int actor, int role){
    // If we have actor 1, check if the role plays in a scene with actor 2
    if(actor == 1 ){
        for(int i=0;i<actorsVector.at(2).size();i++){ // Roles of actor 2
            if(checkRolesInSameScene(actorsVector.at(2).at(i), role)){
                return true; // Has clashing roles --> return true 
            }
        }
    }
    // If we have actor 2, check if the role plays in a scene with actor 1
    if(actor == 2){
        for(int i=0;i<actorsVector.at(1).size();i++){       // Roles of actor 1
            if(checkRolesInSameScene(actorsVector.at(1).at(i), role)){
                return true; // Has clashing roles --> return true 
            }
        }
    }
    // Go through the actor's roles and check if the new role clashes with any of them 
    for(int i=0;i<actorsVector.at(actor).size();i++){
        if(checkRolesInSameScene(actorsVector.at(actor).at(i), role)){
            return true;  // Has clashing roles --> return true 
        }
    }
    return false;
}

/*
    Reads the input. 
*/
void readInput(){
    // Read the first three lines: roles, scenes, actors
    cin >> n >> s >> k;

    // Construct role constraints vector    
    role_constraints.push_back(vector<int>()); // Dummy in index 0 so that we have 1-indexing

    // Go through the role constraints and save them in the vector
    for(int i = 0; i < n; i++){
        int following_numbers;
        cin >> following_numbers;

        vector<int> constraint;
        for (int i = 0; i < following_numbers; i++) {
            int currConstraint;
            cin >> currConstraint;
            constraint.push_back(currConstraint);
        }
        role_constraints.push_back(constraint);
    }

    // Construct scene constraints vector    
    scene_constraints.push_back(vector<int>()); // Dummy in index 0 so that we have 1-indexing

    // Go through the scene constraints and save them in the vector
    for(int i = 0; i < s; i++){
        int following_numbers;
        cin >> following_numbers;

        vector<int> constraint;
        for (int i = 0; i < following_numbers; i++) {
            int currConstraint;
            cin >> currConstraint;
            constraint.push_back(currConstraint);
        }
        scene_constraints.push_back(constraint);
    }

    // Define size of actor vector
    actorsVector.resize(k+1); // 1-indexed. The size will increase again when super actors are added. 
}

/*
    Prints the output.  

*/
void writeOutput(){
    int numOfActors = 0;
    // Go through the list of actors and count how many have been given a role
    for(int i = 1; i < actorsVector.size(); i++){
        if(!actorsVector.at(i).empty()){
            numOfActors++;
        }
    }
    
    // Print number of actors that have been given a role
    cout << numOfActors << endl;
    
    // Go through the actors vector and print the roles of each actor
    for(int i = 1; i < actorsVector.size(); i++){
        if(!actorsVector.at(i).empty()){
            // Print: Number of current actor and number of roles they were given
            cout << i << " " << actorsVector.at(i).size();
            // Print: The roles of the actor
            for(int j = 0; j < actorsVector.at(i).size();j++){
                cout << " " <<  actorsVector.at(i).at(j);
            }
            cout << endl;
        }
    }
}

/*
    Order of execution: 
    1. Read the input
    2. Find all possible roles for the divas 
    3. Find a possible role combination where they are never in the same scene. 
    4. Go through remaining roles and give to the first best available actor. 
    5. Remaining roles are given to super actors
    6. Print the output.   
*/
int main(int argc, char const *argv[]) {
    
    // Step 1: Read the input
    readInput();

    // Step 2: Find all possible roles for the divas 
    vector<int> divaOne; 
    vector<int> divaTwo;
    
    for(int i=1;i<role_constraints.size();i++){
        auto one = find(role_constraints.at(i).begin(), role_constraints.at(i).end(), 1);
        auto two = find(role_constraints.at(i).begin(), role_constraints.at(i).end(), 2);

        if (one != role_constraints.at(i).end()) { // The role can be given to actor 1
            divaOne.push_back(i);
        } 
        if(two != role_constraints.at(i).end()){ // The role can be given to actor 2
            divaTwo.push_back(i);
        }       
    }

    // Step 3: Find a possible role combination where they are never in the same scene.
    for(int i=0;i<divaOne.size();i++){
        bool roleFound = false;
        for(int j=0;j<divaTwo.size();j++){
            
            // If the two roles are not in the same scene, the role assignment works. 
            if(!checkRolesInSameScene(divaOne.at(i),divaTwo.at(j))){
                actorsVector.at(1).push_back(divaOne.at(i));
                actorsVector.at(2).push_back(divaTwo.at(j));
                role_constraints.at(divaOne.at(i)).clear(); // Clear so that we don't iterate through this role again
                role_constraints.at(divaTwo.at(j)).clear();
                roleFound = true;
                break;
            }
        }
        if(roleFound) break;
    }

    // Step 4: Go through remaining roles and give to the first best available actor. 
    for(int i = 1;i<role_constraints.size();i++){
        if(!role_constraints.at(i).empty()){
            // Go through actors that can be given the role
            for(int j = 0; j < role_constraints.at(i).size(); j++){
                int currentActor = role_constraints.at(i).at(j);
            
                // If no clashing roles, give the role to the actor
                if(!checkActorHasClashingRole(currentActor, i)){
                    actorsVector.at(currentActor).push_back(i); 
                    role_constraints.at(i).clear(); // Clear so that we don't iterate through this role again
                    break;  
                }
            }
        }
    } 
    // Step 5: Remaining roles are given to super actors
    int super_actor = k+1; // Initially, the first super actor is in position k+1 of the actors vector
    for(int i = 1;i<role_constraints.size();i++){
        if(!role_constraints.at(i).empty()){
            // Create new actor
            vector<int> newActor; 
            actorsVector.push_back(newActor);

            // Give the role to the actor
            actorsVector.at(super_actor).push_back(i); 
            role_constraints.at(i).clear();
            super_actor++;  
        }
    }

    // Step 6: Print the output.
    writeOutput();

    return EXIT_SUCCESS;
}