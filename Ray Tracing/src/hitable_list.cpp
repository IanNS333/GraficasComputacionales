#include "hitable_list.h"

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record tempRec;
	bool hitAnything = false;
	double closest = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest, tempRec)) {
			hitAnything = true;
			closest = tempRec.t;
			rec = tempRec;
		}
	}

	return hitAnything;
}