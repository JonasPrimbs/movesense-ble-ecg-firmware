#pragma once
/****************************************************************************

        Copyright (c) Suunto Oy 2013-2015.
        All rights reserved.

****************************************************************************/
/**
   @file Compass3D.hpp
   Compass 3D adaptive algorithm to calibrate compass.
*/

#include <whiteboard/builtinTypes/Vector3D.h>
#include <whiteboard/builtinTypes/Matrix.h>

namespace nea
{

class Compass3D FINAL
{
protected:
    class OffsetMinMax
    {
    public:
        float mMin;
        float mMax;

        OffsetMinMax() : mMin(0), mMax(0) {}
        OffsetMinMax(const OffsetMinMax& minMax) : mMin(minMax.mMin), mMax(minMax.mMax) {}
        ~OffsetMinMax() {}
    };

public:
    typedef whiteboard::FloatVector3D Vector3D;
	typedef whiteboard::Matrix Matrix;

    Compass3D();
    ~Compass3D() { }

    /**
        Set for recalibration
    */
    void reCalibrate(void);

    /**
       Start calibration from start (clear previous points), but leave last
       offset alive (=compass could be usable).
       Normaly called before go compass mode.
     */
    void clear(void);


    /**
       Calculate heading

       @param mag	Reference to the magnetic field value
       @param tilt	Reference to the tilt angles

       @return float	Heading in radians 0...2*pi (negative is invalid heading)
     */
    float calculateHeading(const Vector3D& mag, const Vector3D& tilt);

    /**
       Compass offset update

       @param mag	Reference to the source data
     */
    void updateOffset(const Vector3D& mag);

	/**
		Update points for lstsq

	*/

    void updateForLstsq(const Vector3D& rMag, Vector3D& rOffset);

	/**
		Calibration points to matrix and vector
		
	*/

	bool decompose(Matrix& ata, Matrix& L, Matrix& P);

	/**
		Forward-backward solution to LU

	*/

	bool luEvaluate(const Matrix& ata, const Matrix& L, const Matrix& P, Matrix& atb);


    /**
       Set/restore compass offset value

       @param offset Reference to the valid offset
     */
    void setOffset(const Vector3D& offset);

    /**
       Get vector offset

       @return const Vector3D&	Reference to the vector offset
     */
    const Vector3D& getOffset() const { return mOffset; }

    /**
       Get vector length

       @return float	Current vector length uT
     */
    float getVectorLength() const { return mVectorLength; }

    /**
       Get inclination angle

       @return float	Inclination angle in radians
     */
    inline float getInclination() const { return mInclination; }

    /**
       Tell is there valid offset

       @return bool	True if valid offset
     */
    bool isOffsetOk() const;

    /**
       Get volume valid count value (for algorithm debugging)

       @return int 	Volume count
     */
    inline int getVolumeCount() const { return mVolumeCount; }

    /**
       Get angle count value (for algorithm debugging)

       @return int 	Angle count
     */
    inline int getAngleCount() const { return mAngleCount; }

protected:
    /**
       Calculate tilt compensation

       @param mag	Reference to the magnetic field values to compansate
       @param tilt	Reference to the tilt angles

       @return bool	True if compensation succesfully done.
     */
    bool tiltCompensation(Vector3D& mag, const Vector3D& tilt);

    /**
       Calculate source points quality

       @return int	2=Valid, 1=UpdateVector, 0=UpdateStartPoint
     */
    int sourcePointsQuality() const;

    /**
       Check is vector length inaf for update.

       @return bool	True if length inaf
     */
    bool isNewVectorLengthEnough() const;

    /**
        Check is vectors angle valid

        @return bool	True if valid angle
    */
    bool isVectorAngleValid(const Vector3D &rVecA, const Vector3D &rVecB) const;

    /**
       Try update offset.
     */
    void updateAlgorithm();

    /**
       Calculate vector accept length.

       @return float	Accept length.
     */
    float vectorAcceptLength() const;

#ifdef OFFSET_DIFF_LIMITING
    /**
       Calculate offset accept limit.

       @return float	Accept limit.
     */
    float offsetAcceptLimit() const;
#endif

    /**
       Set min & max start value

       @param minMax	Target to update
       @param value		Value to set
     */
    void setMinMaxValue(OffsetMinMax& minMax, float value);

    /**
       Update min & max value

       @param minMax	Target to update
       @param value		Value to update
     */
    void updateMinMaxValue(OffsetMinMax& minMax, float value);

private:
    Compass3D(Compass3D&);
    Compass3D& operator=(Compass3D&);

protected:
    Vector3D mOffset;    // current offset
    float mVectorLength; // current vector length
    float mInclination;  // current inclination angle
    bool mOffsetOk;        //
    Matrix ata2, atb2;
    Vector3D mPoints[6];   // calibration points
    OffsetMinMax mMinMaxX; // min & max values for X-axel
    OffsetMinMax mMinMaxY; // min & max values for Y-axel
    OffsetMinMax mMinMaxZ; // min & max values for Z-axel
    int mCollectStatus;           // algorithm state (number of points)
    int mLstsqStatus;           // 2.0 algorithm state (number of points)
    int mVolumeCount; // volume valid count
    int mAngleCount;  // angle valid count
};

} // namespace nea
