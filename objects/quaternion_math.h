#ifndef _QUATERNION_MATH_H_
#define _QUATERNION_MATH_H_


#include "math.h"

#include "WString.h"

#include "vector_math.h"



#ifndef PI
#define PI 3.1415926541f
#endif

#ifndef DEGREES
#define DEGREES 180.0f/PI
#endif



class Quaternion {
    public:
    
        float w;
        float x;
        float y;
        float z;

        bool valid = true;
        
        Quaternion() {
            w = 0.0f;
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        /**
         * Creates a Quaternion where the rotational axis
         * is around the given Vector (Does not need to be a unit vector)
         * and the rotation of "angle" [Rad].
         *
         * @param values axis[Vector] and angle[Rad]
         * @return none.
         */
        Quaternion(Vector axis, float angle) {

            if (axis.isZeroVector()) {

                *this = Quaternion(1,0,0,0);

            } else {

                axis.normalize();

                angle /= 2.0f;
                float sa = sinf(angle);
                
                w = cosf(angle);
                x = axis.x*sa;
                y = axis.y*sa;
                z = axis.z*sa;

            }

        }


        Quaternion(const Vector &vector) {
    
            w = 0;
            x = vector.x;
            y = vector.y;
            z = vector.z;

        }
        

        Vector toVector() {
            return Vector(x,y,z);
        }

        
        Quaternion(const float &nw, const float &nx, const float &ny, const float &nz) {
            this->w = nw;
            this->x = nx;
            this->y = ny;
            this->z = nz;
        }

        /**
         * Simpler version of Quaternion(float nw, float nx, float ny, float nz)
         * but w is set to 0.
         *
         * @param values x, y and z
         * @return none.
         */

        Quaternion(const float &nx, const float &ny, const float &nz) {
            this->w = 0.0f;
            this->x = nx;
            this->y = ny;
            this->z = nz;
        }

        /**
         * Conjugates the Quaternion.
         * Will change values of quaternion.
         * USe .copy() to keep values.
         * 
         * @param values none
         * @return copy of conjugated Quaternion.
         */
        Quaternion& conjugate() {
            x = -x;
            y = -y;
            z = -z;
            return *this;
        }

        /**
         * Computes the Magnitude (Length) of
         * the Quaternion
         *
         * @param values none
         * @return Magnitude.
         */
        float getMagnitude() {

            return sqrtf(w*w + x*x + y*y + z*z);

        }

        /**
         * Computes the axis[Unit Vector] of rotation and angle[Rad].
         *
         * @param values pointer to axis Vector and angle float
         * @return none.
         */
        void getAxisAngle(Vector &axis, float &angle) {

            axis.x = this->x;
            axis.y = this->y;
            axis.z = this->z;

            axis.normalize();

            angle = acosf(w)*2;

        }

        /**
         * Normalises the Quaternion (Gives length of 1).
         * If sign is true then w will be positive
         * (Makes calculations sometimes more reliable)
         *
         * @param values sign
         * @return normalized Quaternion.
         */
        Quaternion& normalize(const bool &sign = false) {

            float m = getMagnitude();

            if (true) {

                if (sign && w < 0.0f) m = -m;
                
                w /= m;
                x /= m;
                y /= m;
                z /= m;

            }

            if (w==w && x==x && y==y && z==z) return *this; //Check for NAN event

            //valid = false;

            return *this;

        }
        
        /**
         * Returns a copy of the Quaternion
         *
         * @param values none
         * @return copy of Quaternion.
         */
        Quaternion copy() {
            return Quaternion(w,x,y,z);
        }

        /**
         * Returns a copy of the Quaternion
         *
         * @param values none
         * @return copy of Quaternion.
         */
        Vector rotateVector(const Vector &vectorToRotate) {
            return (*this*vectorToRotate*(copy().conjugate())).toVector();
        }

        /**
         * Returns a String containing components.
         * Form:
         * w: ..., x: ..., y: ..., z:...
         * Where ... is the value.
         * Default digits is 2.
         * 
         *
         * @param values digits.
         * @return String.
         */
        String toString(const uint8_t &digits = 2) {
            return "w: " + String(w, digits) + ", x: " + String(x, digits) + ", y: " + String(y, digits) + ", z: " + String(z, digits);  
        }

        Quaternion operator + (const Quaternion &b) {
            return Quaternion(w + b.w, x + b.x, y + b.y, z + b.z);
        }

        Quaternion operator - (const Quaternion &b) {
            return Quaternion(w - b.w, x - b.x, y - b.y, z - b.z);
        }

        Quaternion operator - (void) {
            return Quaternion(-w, -x, -y, -z);
        }

        Quaternion operator * (const float &c) {
            return Quaternion(w*c, x*c, y*c, z*c);
        }

        Quaternion operator / (const float &c) {
            return Quaternion(w/c, x/c, y/c, z/c);
        }

        void operator *= (const float &c) {
            w *= c;
            x *= c;
            y *= c;
            z *= c;
        }

        void operator += (const Quaternion &b) {
            w += b.w;
            x += b.x;
            y += b.y;
            z += b.z;
        }

        Quaternion operator * (const Quaternion &q) {
            return Quaternion(
                w*q.w - x*q.x - y*q.y - z*q.z,  // new w
                w*q.x + x*q.w + y*q.z - z*q.y,  // new x
                w*q.y - x*q.z + y*q.w + z*q.x,  // new y
                w*q.z + x*q.y - y*q.x + z*q.w); // new z
        }

};


/*Quaternion sqrt(Quaternion a) {
    return Quaternion(sqrtf(a.w), sqrtf(a.x), sqrtf(a.y), sqrtf(a.z));
}*/


#endif