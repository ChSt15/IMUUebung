#ifndef _VECTOR_MATH_H_
#define _VECTOR_MATH_H_


#include "math.h"


#ifndef PI
#define PI 3.1415926541f
#endif

#ifndef DEGREES
#define DEGREES (PI/180.0f)
#endif



class Vector {
    public:
    
        float x;
        float y;
        float z;

        bool valid = true;

        Vector() {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        /**
         * This constructor sets all components to n.
         */
        Vector(const float &n) {
            x = n;
            y = n;
            z = n;
        }

        Vector(const float &nx, const float &ny, const float &nz) {
            x = nx;
            y = ny;
            z = nz;
        }

        //Vector(const Quaternion &quaternion);

        /**
         * Calculates the magnitude of the vector.
         *
         * @param values none.
         * @return magnitude of the vector.
         */
        float magnitude() {

            float m = x*x + y*y + z*z;

            m = sqrtf(m);
            
            if (m == m) { // NAN check
                return m;
            }
            valid = false;
            return 0.0f;
        }

        /**
         * Copies the vector.
         *
         * @param values none.
         * @return copy of the vector.
         */
        Vector copy() {
            return Vector(x, y, z);
        }

        /**
         * Normalizes the Vector.
         *
         * @param values none.
         * @return copy of the normalized vector.
         */
        Vector& normalize() {

            float mag = magnitude();

            if (mag != 0.0f) {
                x /= mag;
                y /= mag;
                z /= mag;
            } else {
                x = 0.0f;
                y = 0.0f;
                z = 0.0f;
            }

            return *this;

        }

        /**
         * Checks if vector is zero vector.
         *
         * @param values none.
         * @return true if vecter is zero vector.
         */
        bool isZeroVector() {

            return magnitude() == 0.0f;

        }

        /**
         * Multiplies the vectors components to this scheme:
         * Vector(x*x, y*y, z*z)
         *
         * @param values none.
         * @return vector.
         */
        Vector compWiseMulti(const Vector &vec) {
            return Vector(x*vec.x, y*vec.y, z*vec.z);
        }

        /**
         * Adds the components of the vectors.
         *
         * @param values none.
         * @return copy of the vector.
         */
        Vector operator + (Vector b) {
            return Vector(x + b.x, y + b.y, z + b.z);
        }

        /**
         * Subtracts the components of the vectors.
         *
         * @param values none.
         * @return copy of the vector.
         */
        Vector operator - (Vector b) {
            return Vector(x - b.x, y - b.y, z - b.z);
        }

        /**
         * Negates the components of the vectors.
         *
         * @param values none.
         * @return copy of the vector.
         */
        Vector operator - (void) {
            return Vector(-x, -y, -z);
        }

        /**
         * Scales the vector.
         *
         * @param values none.
         * @return copy of the vector.
         */
        Vector operator * (float c) {
            return Vector(x*c, y*c, z*c);
        }

        /**
         * Divides the components of the vectors.
         *
         * @param values none.
         * @return copy of the vector.
         */
        Vector operator / (float c) {
            return Vector(x/c, y/c, z/c);
        }

        /**
         * Compound scaling.
         *
         * @param values none.
         * @return nothing.
         */
        void operator *= (float c) {
            x *= c;
            y *= c;
            z *= c;
        }

        /**
         * Compound addition.
         *
         * @param values none.
         * @return nothing.
         */
        void operator += (Vector b) {
            x += b.x;
            y += b.y;
            z += b.z;
        }

        /**
         * Vector multiplication.
         * 
         * x1*x2 + y1*y2 + z1*z2
         *
         * @param values none.
         * @return multiplication result in float.
         */
        float operator * (Vector b) {
            return x*b.x + y*b.y + z*b.z;
        }

        /**
         * Vector equals.
         * True is all components are equal
         *
         * @param values none.
         * @return bool.
         */
        bool operator == (Vector b) {
            return x==b.x && y==b.y && z==b.z;
        }

        /**
         * Vector not equals.
         * true if any component not equal
         *
         * @param values none.
         * @return bool.
         */
        bool operator != (Vector b) {
            return !(x==b.x && y==b.y && z==b.z);
        }

        /**
         * Computes cross multiplication of 2 vectors.
         * 
         *
         * @param values none.
         * @return Vector.
         */
        Vector cross(Vector b) {
            return Vector(
                y*b.z-z*b.y,
                z*b.x-x*b.z,
                x*b.y-y*b.x
            );
        } 

        /**
         * Computes angle between 2 vectors.
         * 
         *
         * @param values none.
         * @return angle(float) in radians.
         */
        float getAngleTo(Vector b) {
            
            float ca = (*this)*b/(magnitude()*b.magnitude());

            return acos(ca);

        }


        /**
         * Computes the projection onto another vector.
         * 
         *
         * @param values none.
         * @return Vector.
         */
        Vector getProjectionOn(Vector b) {

            b.normalize();

            return b*(*this*b);

        }


        /**
         * Returns a String containing components.
         * Form:
         * x: ..., y: ..., z:...
         * Where ... is the value.
         * Default digits is 2.
         * 
         *
         * @param values digits.
         * @return String.
         */
};


//Is a vector with a z component of -9.81.
#define GRAVITY_VECTOR Vector(0,0,-9.81)


/*Vector sqrt(Vector a) {
    return Vector(sqrt(a.x), sqrt(a.y), sqrt(a.z));
}*/


#endif
