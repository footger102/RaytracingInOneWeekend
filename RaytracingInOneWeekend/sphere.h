#pragma once

#include "hittable.h"

class sphere : public hittable {
public:
	sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)){}

	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
		vec3 oc = center - r.origin();
		const double& a = r.direction().norm_square();
		const double& h = dot(r.direction(), oc);
		const double& c = dot(oc, oc) - radius * radius;
		const double& discriminant = h * h - a * c;

		if (discriminant < 0) return false;

		//Find the nearest root that lies in the acceptable range.
		double root = (h - std::sqrt(discriminant)) / a;
		if (root < ray_tmin or root > ray_tmax) {
			root = (h + std::sqrt(discriminant)) / a;
			if (root < ray_tmin or root > ray_tmax) return false;
		}

		//Save information of hitted position to rec object 
		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}

private:
	point3 center;
	double radius;
};