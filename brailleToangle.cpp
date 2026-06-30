#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;


//movemotor returns the angle by which we need to move the servoMotor in order to change the position from initial to final 
//it returns -1 if we cant , if the output is +ve then we need to move clockwise , if the output is negative then we need to move 
// anticlockwise
int moveMotorActual(int angle , int currentPosition){
    //move motor by arduino to currentPosition+angle;
}


int moveMotor(int initial, int final){
    if(final==1){
        if(initial==1){
            return 0;
        }else if (initial==2){
            return -120 ;//anticlockwise 120
        }else if (initial==3){
            return 120; //clockwise 120;
        }else if (initial==12){
            return -60; //anticlockwise 60
        }else if (initial==13){
            return +60; //clockwise 60
        }
        else return -1; //cant move YE CASE KABHI AANA HI NAHI CHAHIYE JUST FOR SAFETYPURPOSES
    }
     if(final==2){
        if(initial==1){
            return 120; //clockwise 120
        }else if (initial==2){
            return 0;//do nothing ( YE CASE KABHI AANA NAHI CHAHIYE LIKH DIYA FOR SAFETY)
        }else if (initial==3){
            return -120; //anticlockwise 120;
        }else if (initial==12){
            return 60; //clockwise 60
        }else if (initial==23){
            return -60; //anticlockwise 60
        }else return -1; //YE CASE BHI KABHI NAHI AANA CHAHIYE 
    }
     if(final==3){
        if(initial==1){
            return -120; //anticlockwise 120
        }else if (initial==2){
            return 120 ;//clockwise 120
        }else if (initial==3){
            return 0; //YE CASE KABHI NAHI AANA CHAHIYE 
        }else if (initial==23){
            return 60; //clockwise 60
        }else if (initial==13){
            return -60; //anticlockwise 60
        }else return -1;
    }
    if(final == 12){
        if(initial==1){
            return +60; //clockwise 60
        }else if (initial==2){
            return -60;//anticlockwise 60
        }else if (initial==3){
            return -1; //YE CASE KABHI NAHI AANA CHAHIYE 
        }else if (initial==23){
            return -1; 
        }else if (initial==13){
            return -1;
        }else return -1;
    }
    if(final == 23){
        if(initial==1){
            return -1; 
        }else if (initial==2){
            return +60;//clockwise 60
        }else if (initial==3){
            return -60; //YE CASE KABHI NAHI AANA CHAHIYE 
        }else if (initial==23){
            return 0; 
        }else if (initial==13){
            return -1;
        }else return -1;
    }
    if(final == 13){
        if(initial==1){
            return -60; //anticlockwise 60
        }else if (initial==2){
            return -1; //NOT POSSIBLE
        }else if (initial==3){
            return +60; //CLOCKWISE 60
        }else if (initial==23){
            return -1; 
        }else if (initial==13){
            return -1;
        }else return -1;
    }
}









int main(){
    vector<int> answer;
    vector<string> data={"110","010","100","010","111","000","111","000","101","010"};
    //we will print a temporary verticle line (100);
    int currentPosition = 1;
    int firstNonZero=-1;
    for (int i = 0; i <data.size(); i++)
    {
        if(data[i][0]=='1'||data[i][1]=='1'||data[i][2]=='1'){
            firstNonZero=i;
            break;
        }
    }
    if(firstNonZero!=-1){
        if(data[firstNonZero][0]=='1'){
            int angle =moveMotor(1,12);
            //move motor by angle;
            cout<<angle<<" ";
            answer.push_back(angle);
            currentPosition=12;
        }else if (data[firstNonZero][1]=='1'){
            int angle = moveMotor(1,12);
            //move motor by angle
            answer.push_back(angle);
            cout<<angle<<" ";
            currentPosition=12;
        }else if (data[firstNonZero][2]=='1'){
            int angle =moveMotor(1,13);
            //move motor by angle
            answer.push_back(angle);
            cout<<angle<<" ";
            currentPosition=13;
        }
    }
    answer.push_back(500);
    //move the setup
    cout<<currentPosition<<"This is the current position Before the start "<<endl;
    for (int i = 0; i <data.size()-1; i++)
    {
        //move the setup forward (from the initial temporary printed letter to the ith );
        if(i!=0)answer.push_back(500);
        cout<<"index = "<<i<<"    ";
        if(data[i][0]=='1'){
            int angle=moveMotor(currentPosition,1);
            //move motor by angle;
            answer.push_back(angle);
            cout<<"currentPosition "<<currentPosition<<" "<<"angleToMove "<<angle<<"finalPosition ";
            currentPosition=1;
            cout<<currentPosition<<endl<<endl;
        }
        if (data[i][1]=='1'){
            int angle =moveMotor(currentPosition,2);
            //move motor by angle
            answer.push_back(angle);
            cout<<"currentPosition "<<currentPosition<<" "<<"angleToMove "<<angle<<"finalPosition ";
            currentPosition=2;
            cout<<currentPosition<<endl<<endl;
        }
        if (data[i][2]=='1'){
            int angle = moveMotor(currentPosition,3);
            //move motor by angle
            answer.push_back(angle);
            cout<<"currentPosition "<<currentPosition<<" "<<"angleToMove "<<angle<<"finalPosition ";
            currentPosition=3;
            cout<<currentPosition<<endl<<endl;

        }
        int nextNonZero=-1;
        //change the current position for next verticle line;
        for(int j=i+1;j<data.size();j++){
            if(data[j][0]=='1'||data[j][1]=='1'||data[j][2]=='1'){
                nextNonZero=j;
                break;
            }
        }
        if(nextNonZero==-1){
            int angle1=moveMotor(currentPosition,12);
            int angle2=moveMotor(currentPosition,13);
            if(angle1!=-1){
                //move motor by angle1;
                cout<<angle1<<" ";
                answer.push_back(angle1);
                currentPosition=12;
            }else{
                //move motor by angle2;
                cout<<angle2<<" ";
                answer.push_back(angle2);
                currentPosition=13;
            }
        }
        else
        {
            if(data[nextNonZero][0]=='1'){
                int angle1 = moveMotor(currentPosition,12);
                int angle2=moveMotor(currentPosition,13);
                if(angle1!=-1){
                    cout<<"getting ready for next row angle to rotate : "<<angle1<<" ";
                    //move motor by angle1;
                    currentPosition=12;
                    answer.push_back(angle1);
                    cout<<currentPosition<<endl<<endl;
                }else
                {
                    //move motor by angle2;
                    cout<<"getting ready for next row angle to rotate : "<<angle2<<" ";
                    answer.push_back(angle2);
                    currentPosition=13;
                    cout<<currentPosition<<endl<<endl;
                }   
            }
            else if (data[nextNonZero][1]=='1'){
                int angle1 = moveMotor(currentPosition,12);
                int angle2=moveMotor(currentPosition,23);
                if(angle1!=-1){
                    //move motor by angle1;
                    cout<<"getting ready for next row angle to rotate : "<<angle1<<" ";
                    answer.push_back(angle1);
                    currentPosition=12;
                    cout<<currentPosition<<endl<<endl;
                }else
                {
                    //move motor by angle2;
                    cout<<"getting ready for next row angle to rotate : "<<angle2<<" ";
                    answer.push_back(angle2);
                    currentPosition=23;
                    cout<<currentPosition<<endl<<endl;
                }   
            }
            else {
                int angle1 = moveMotor(currentPosition,23);
                int angle2=moveMotor(currentPosition,13);
                if(angle1!=-1){
                    //move motor by angle1;
                    cout<<"getting ready for next row angle to rotate : "<<angle1<<" ";
                    answer.push_back(angle1);
                    currentPosition=23;
                    cout<<currentPosition<<endl<<endl;
                }else
                {
                    //move motor by angle2;
                    cout<<"getting ready for next row angle to rotate : "<<angle2<<" ";
                    currentPosition=13;
                    answer.push_back(angle2);
                    cout<<currentPosition<<endl<<endl;
                }   
            }
            
        }
        //PRINTING EVERY VERTICLE LINE EXCEPT THE LAST 1 
        
        
        //current position is updated and all set for the next verticle line and we have already moved our motor by the required angle
        // one thing we can do is to calculate the angle which we shall move and then move it after all if else hell
        //NOW MOVE THE SETUP TOWARDS RIGHT .s
        cout<<"movingToNextRow"<<endl;
    }
    
    cout<<"PRINTING THE ANSWER"<<endl;
    for (int i = 0; i <answer.size(); i++)
    {
        cout<<answer[i]<<" ";
    }
    cout<<"HELLo";
    return 0;
}


// encodings to angle convertor