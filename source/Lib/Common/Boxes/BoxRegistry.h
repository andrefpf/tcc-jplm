#ifndef BOXREGISTRY_H__
#define BOXREGISTRY_H__

class BoxRegistry
{
// std::map<TBox, value> map;

public:
	BoxRegistry();
	~BoxRegistry();
	

	void add_to_registry(const Box& box);
	bool is_known(const TBox& t_box) const noexcept;
};


#endif /* end of include guard: BOXREGISTRY_H__ */