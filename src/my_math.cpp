//
// Created by zhn on 25-11-27.
//
#include<my_math.h>
using namespace my_math;

const int index3[] = {0,1,2,0,1,2};
vec2 EraseVector(int i,vec3& t){
	return vec2(t[index3[i+1]],t[index3[i+2]]);
}

bool RayAABBIntersection(vec2 origin, vec2 dir, vec2 aabbMin, vec2 aabbMax){
	cout<<"-------------------test2---------------"<<endl;
	Debug2(origin);
	Debug2(dir);
	Debug2(aabbMin);
	Debug2(aabbMax);
	if(abs(dir.x)<epsf){
		if(abs(dir.y)<epsf)return false;
		if(origin.x<aabbMin.x||origin.x>aabbMax.x)return false;
		return true;
	}else if(abs(dir.y)<epsf){
		if(origin.y<aabbMin.y||origin.y>aabbMax.y)return false;
		return true;
	}else{
		vec2 t1 = (aabbMin-origin) / dir;
		vec2 t2 = (aabbMax-origin) / dir;
		vec2 tmin = glm::min(t1,t2);
		vec2 tmax = glm::max(t1,t2);
		if(tmin.x > tmax.y||tmin.y > tmax.x)return false;
		return true;
	}
}

bool RayAABBIntersection(vec3 origin, vec3 dir, vec3 aabbMin, vec3 aabbMax){
	cout<<"-------------------test3---------------"<<endl;
	Debug3(origin);
	Debug3(dir);
	Debug3(aabbMin);
	Debug3(aabbMax);
	for(int i=0;i<3;i++){
		if(abs(dir[i])<epsf){
			if(origin[i]<aabbMin[i]||origin[i]>aabbMax[i])return false;
			return RayAABBIntersection(EraseVector(i,origin),EraseVector(i,dir),EraseVector(i,aabbMin),EraseVector(i,aabbMax));
		}
	}
	cout<<1<<endl;
	vec3 t1 = (aabbMin-origin) / dir;
	vec3 t2 = (aabbMax-origin) / dir;
	vec3 tmin = glm::min(t1,t2);
	vec3 tmax = glm::max(t1,t2);
	Debug3(t1);
	Debug3(t2);
	Debug3(tmin);
	Debug3(tmax);
	float actual_tmin = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
    float actual_tmax = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

    cout << "actual_tmin: " << actual_tmin << endl;
    cout << "actual_tmax: " << actual_tmax << endl;

    // 如果最大tmin <= 最小tmax，且tmax >= 0（考虑射线方向），则相交
    return actual_tmin <= actual_tmax && actual_tmax >= 0.0f;
}
