#ifndef JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__
#define JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__

template <typename T>
class BoxDataHolder {
protected:
	T value;
public:
	auto get_value() {
		return value;
	}

	void set_value(T value) {
		this->value = value;
	}

	virtual ~BoxDataHolder(){}
	
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__ */