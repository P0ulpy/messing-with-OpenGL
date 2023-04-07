//
// Created by Flo on 07/04/2023.
//

#pragma once

template<typename T>
struct Point2d
{
    Point2d(const T x = 0, const T y = 0): x(x), y(y) {}
    Point2d(const Point2d& pt): x(pt.x), y(pt.y) {}

    T x;
    T y;
};

template<typename T>
struct Point3d
{
    Point3d(const T x = 0, const T y = 0, const T z = 0): x(x), y(y), z(z) {}
    Point3d(const Point3d& pt): x(pt.x), y(pt.y), z(pt.z) {}

    T x;
    T y;
    T z;
};

template<typename T, size_t Size>
struct Mat2D
{
    Mat2D() { for(int i = 0; i < Size * Size ; ++i) m_data[i] = 0; }

    T& operator() (const int i, const int j) { return m_data[j * Size + i]; }
    const T& operator() (const int i, const int j) const { return m_data[j * Size + i]; }

    std::array<T, 16> m_data;
};

template<typename T, size_t Size>
Mat2D<T, Size> operator*(const Mat2D<T, Size>& lhs, const Mat2D<T, Size>& rhs)
{
    Mat2D<T, Size> result;
    for (int i = 0; i < Size; ++i)
    {
        for (int j = 0; j < Size; ++j)
        {
            for (int k = 0; k < Size; ++k)
            {
                result(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return result;
}

template<typename Type>
struct Mat4
{
    Mat4() { for(int i = 0; i < 16 ; ++i) m_data[i] = 0; }

    Type& operator() (const int i, const int j) { return m_data[j * 4 + i]; }
    const Type& operator() (const int i, const int j) const { return m_data[j * 4 + i]; }

    const Type* data() const { return m_data.data(); }

    static Mat4<Type> identity()
    {
        Mat4<Type> result;
        for (int i = 0; i < 4; ++i)
        {
            result(i, i) = 1;
        }
        return result;
    }

    static Mat4<Type> rotationX(const Type& angle)
    {
        return rotationAxisAligned<1, 2>(angle);
    }

    static Mat4<Type> rotationY(const Type& angle)
    {
        return rotationAxisAligned<0, 2>(angle);
    }

    static Mat4<Type> rotationZ(const Type& angle)
    {
        return rotationAxisAligned<0, 1>(angle);
    }

    static Mat4<Type> translation(const Type& x, const Type& y, const Type& z)
    {
        Mat4<Type> result = identity();
        result(0, 3) = x;
        result(1, 3) = y;
        result(2, 3) = z;
        return result;
    }

    static Mat4<Type> translation(const Point3d<Type>& pt)
    {
        return translation(pt.x, pt.y, pt.z);
    }

    static Mat4<Type> projection(const Type& aspect, const Type& fov, const Type& farPlane, const Type& nearPlane)
    {
        Mat4<Type> result;
        result(0, 0) = 1 / (aspect * std::tan(fov / 2));
        result(1, 1) = 1 / std::tan(fov / 2);
        result(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result(2, 3) = -2 * farPlane * nearPlane / (farPlane - nearPlane);
        result(3, 2) = -1;
        return result;
    }

private:
    template <size_t a1, size_t a2>
    static Mat4<Type> rotationAxisAligned(const Type& angle)
    {
        Mat4<Type> result = identity();
        result(a1, a1) = std::cos(angle);
        result(a1, a2) = std::sin(angle);
        result(a2, a1) = -std::sin(angle);
        result(a2, a2) = std::cos(angle);
        return result;
    }

    std::array<Type, 16> m_data;
};

template<typename T>
Mat4<T> operator*(const Mat4<T>& lhs, const Mat4<T>& rhs)
{
    Mat4<T> result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                result(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return result;
}