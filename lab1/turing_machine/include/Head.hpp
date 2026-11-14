#ifndef HEAD_HPP
#define HEAD_HPP

class Head {
private:
    int position_;

public:
    explicit Head(int start_pos = 0);

    int getPosition() const;
    void setPosition(int pos);

    Head& operator++();
    Head operator++(int);
    Head& operator--();
    Head operator--(int);

    bool operator==(const Head& other) const;
    bool operator!=(const Head& other) const;
};

#endif