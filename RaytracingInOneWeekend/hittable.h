#pragma once
#include "rtweekend.h"

class material; //Prevent circular reference

class hit_record {
public:
	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		//Sets the hit record normal vector.
		//NOTE : the parameter 'outward_normal' is assumed to have unit length

		front_face = dot(r.direction(), outward_normal) < 0; //Decides whether ray and outward normal vector is opposite direction.
		normal = front_face ? outward_normal : -outward_normal; //Setup to keep always opposite direction.
	}
};

class hittable {
public:
	virtual ~hittable() = default; //deconstructor can be justified inherited class

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0; //pure virtual function. This should be justified in inherited class
};