#pragma once

#include "hittable.h"

class sphere : public hittable {
public:
	sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat){
		//Initialize the material pointer 'mat'

	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		vec3 oc = center - r.origin();
		const double& a = r.direction().norm_square();
		const double& h = dot(r.direction(), oc);
		const double& c = dot(oc, oc) - radius * radius;
		const double& discriminant = h * h - a * c;

		if (discriminant < 0) return false;

		//Find the nearest root that lies in the acceptable range.
		double root = (h - std::sqrt(discriminant)) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + std::sqrt(discriminant)) / a;
			if (!ray_t.surrounds(root)) return false;
		}

		//Save information of hitted position to rec object 
		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;

		return true;
	}

private:
	point3 center;
	double radius;
	shared_ptr<material> mat;
};