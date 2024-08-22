#include "color.h"
#include "vec3.h"
#include "ray.h"

using namespace std;

double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = center - r.origin();
	const double& a = r.direction().norm_square();
	const double& h = dot(r.direction(), oc);
	const double& c = dot(oc, oc) - radius * radius;
	const double& discriminant = h * h - a * c;

	if (discriminant < 0) return -1.0;
	else return (h - sqrt(discriminant)) / a;
}

color ray_color(const ray& r) {
	//Reutrns the color for a given scene ray
	const point3& sphere_center = point3(0, 0, -1);
	const double& t = hit_sphere(sphere_center, 0.5, r);
	if (t > 0.0) {
		vec3 N = unit_vector(r.at(t) - sphere_center); //normal unit vector of each hit position
		return 0.5 * color(N[0] + 1, N[1] + 1, N[2] + 1);
	}

	//Background
	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction[1] + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
	//Image
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	//Calculate the image height, and ensure that it's at least 1.
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	//Camera
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = double(viewport_height * image_width / image_height); //viewport_width should be double type!!
	auto camera_center = point3(0, 0, 0);

	//Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	//Calculate the horizontal and vertical delta vectors from pixel to pixel.
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	//Calculate the horizontal and vertical delta vectors from pixel to pixel.
	const auto& viewport_upper_left = camera_center - vec3(0, 0, focal_length) - 0.5 * viewport_u - 0.5 * viewport_v;
	const auto& pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	//Render
	cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		clog << "\rScanlines remaining: " << (image_height - j) << " " << flush;
		for (int i = 0; i < image_width; i++) {
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}
	clog << "\rDone.				\n";
}