#ifndef KTH_COMPARE
#define KTH_COMPARE

template <typename T>
class MoreCompare {
public:
    bool operator()(const T &left, const T &right);
};

#endif // KTH_COMPARE
