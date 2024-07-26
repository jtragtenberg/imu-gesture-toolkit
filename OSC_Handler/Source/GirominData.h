/*
  ==============================================================================

    GirominData.h
    Created: 26 Jul 2024 4:52:38pm
    Author:  Solomon Moulang Lewis

  ==============================================================================
*/

#pragma once

#include <array>
#include <string>

class GirominData
{
public:
    GirominData()
    {
        id_ = 0;
        is_active_ = false;
        addr_ = "";
        
        std::fill (a_.begin(), a_.end(), 0.0f);
        std::fill (g_.begin(), g_.end(), 0.0f);
        std::fill (q_.begin(), q_.end(), 0.0f);
        std::fill (b_.begin(), b_.end(), 0.0f);
    }
    
    ~GirominData() {}
    
    // SETTERS
    void setID (int id) { id_ = id; }
    
    void setIsActive (bool is_active) { is_active_ = is_active; }
    
    void setAX (float value) { a_[0] = value; }
    void setAY (float value) { a_[1] = value; }
    void setAZ (float value) { a_[2] = value; }
    
    void setGX (float value) { g_[0] = value; }
    void setGY (float value) { g_[1] = value; }
    void setGZ (float value) { g_[2] = value; }
    
    void setQ1 (float value) { q_[0] = value; }
    void setQ2 (float value) { q_[1] = value; }
    void setQ3 (float value) { q_[2] = value; }
    void setQ4 (float value) { q_[3] = value; }
    
    void setB1 (float value) { b_[0] = value; }
    void setB2 (float value) { b_[1] = value; }
    
    // GETTERS
    float getAX() { return a_[0]; }
    float getAY() { return a_[1]; }
    float getAZ() { return a_[2]; }
    
    float getGX() { return g_[0]; }
    float getGY() { return g_[1]; }
    float getGZ() { return g_[2]; }
    
    float getQ1() { return q_[0]; }
    float getQ2() { return q_[1]; }
    float getQ3() { return q_[2]; }
    float getQ4() { return q_[3]; }
    
    float getB1() { return b_[0]; }
    float getB2() { return b_[1]; }
    
private:
    int id_;
    // maybe should have its orientation stored in here
    bool is_active_;
    std::string addr_;
    std::array<float, 4> q_;
    std::array<float, 3> a_;
    std::array<float, 3> g_;
    std::array<float, 2> b_;
};
