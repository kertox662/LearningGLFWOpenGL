#include <glm/glm.hpp>
#include <unordered_map>

class Camera{
    float sensitivity, pitchLim;
    float yaw,pitch,roll;
    glm::vec3 pos;
    glm::vec3 front, up;
    glm::mat4 view;

    public:
    Camera();
    inline const glm::mat4& getView(){return view;}
    
    void updateView();
    void updateFront();
    inline glm::vec3& getFront(){return front;}
    inline glm::vec3& getUp()   {return up;}

    void setYaw(float y);
    void setPitch(float p);
    void setRoll(float r);

    void addYaw(float dy);
    void addPitch(float dp);
    void addRoll(float dr);

    inline const float getYaw(){return yaw;}
    inline const float getPitch(){return pitch;}
    inline const float getRoll(){return roll;}

    void moveCamera(std::unordered_map<int,bool> keys, float dt);
    inline const glm::vec3& getPos(){return pos;}

    void setSensitivity(float s);
    void setPitchLimit (float pl);
    inline const float getSensitivity(){return sensitivity;}
    inline const float getPitchLimit(){return pitchLim;}
    inline const void constrainPitch(){
        if (pitch > pitchLim) pitch = pitchLim;
        else if(pitch < -pitchLim) pitch = -pitchLim;
    }
};