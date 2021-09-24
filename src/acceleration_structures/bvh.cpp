#include "bvh.h"

inline bool bounding_box_compare(const Hittable *a, const Hittable *b, int axis) {
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
    	return false;

    return box_a.mn[axis] < box_b.mn[axis];
}


bool box_x_compare(const Hittable *a, const Hittable *b) {
    return bounding_box_compare(a, b, 0);
}

bool box_y_compare(const Hittable *a, const Hittable *b) {
    return bounding_box_compare(a, b, 1);
}

bool box_z_compare(const Hittable *a, const Hittable *b) {
    return bounding_box_compare(a, b, 2);
}


BVH_Node::BVH_Node():
left(nullptr),
right(nullptr),
box()
{}

BVH_Node::BVH_Node(HittableList &hitlist):
BVH_Node(hitlist, 0, hitlist.size())
{}

BVH_Node::BVH_Node(HittableList &hitlist, size_t from, size_t to) {
	int axis = 0;
	double est_x = BVH_Node_by_axis_estimation(hitlist, from, to, 0);
	double est_y = BVH_Node_by_axis_estimation(hitlist, from, to, 1);
	double est_z = BVH_Node_by_axis_estimation(hitlist, from, to, 2);

	if (est_x <= est_y && est_x <= est_z) {
		axis = 0;
	} else if (est_y <= est_x && est_y <= est_z) {
		axis = 1;
	} else {
		axis = 2;
	}

	auto comparator = (axis == 0) ? box_x_compare
					: (axis == 1) ? box_y_compare
								  : box_z_compare;

	size_t objects_cnt = to - from;
	if (objects_cnt == 1) {
		left = right = hitlist[from];
	} else if (objects_cnt == 2) {
		if (comparator(hitlist[from], hitlist[from + 1])) {
			left  = hitlist[from];
			right = hitlist[from + 1];
		} else {
			right = hitlist[from];
			left  = hitlist[from + 1];
		}
	} else {
		 std::sort(hitlist.hittables.begin() + from, hitlist.hittables.begin() + to, comparator);

		 double mid = from + objects_cnt / 2;
		 left  = new BVH_Node(hitlist, from, mid);
		 right = new BVH_Node(hitlist, mid , to);
	}

	AABB box_left, box_right;
	if (!left->bounding_box(box_left) || !right->bounding_box(box_right)) {
		exit(0);
	}

	box = surrounding_box(box_left, box_right);
}

double BVH_Node_by_axis_estimation(HittableList &hitlist, size_t from, size_t to, const int axis) {
	auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

	size_t objects_cnt = to - from;
	if (objects_cnt <= 1) {
		return 0;
	} else {
		 std::sort(hitlist.hittables.begin() + from, hitlist.hittables.begin() + to, comparator);
		 double mid = from + objects_cnt / 2;
		 AABB left, right;
		 hitlist.bounding_box(left, from, mid);
		 hitlist.bounding_box(right, mid , to);
		 return left.effective_size() + right.effective_size();
	}
}

bool BVH_Node::hit(Ray &ray, HitRecord* hitRecord) const {
	if (!box.hit(ray, 0, VEC3D_INF)) {
		return false;
	}

    bool left_hit = left->hit(ray, hitRecord);
    bool right_hit = right->hit(ray, hitRecord);

    return left_hit || right_hit;
}

bool BVH_Node::bounding_box(AABB &output_box) const {
	output_box = box;
    return true;
}
