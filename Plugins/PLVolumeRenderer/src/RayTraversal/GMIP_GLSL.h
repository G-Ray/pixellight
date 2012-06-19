/*********************************************************\
 *  File: GMIP_GLSL.h                                    *
 *      Fragment shader source code - GLSL (OpenGL 3.3 ("#version 330")
 *
 *  Master thesis
 *    "Scalable Realtime Volume Rendering"
 *
 *  At
 *    Fachhochschule W�rzburg-Schweinfurt
 *    Fakult�t Informatik, Wirtschaftsinformatik (FIW)
 *    http://www.fh-wuerzburg.de/
 *
 *  Author
 *    Christian Ofenberg (c.ofenberg@pixellight.org or cofenberg@googlemail.com)
 *    Copyright (C) 2011-2012
\*********************************************************/


// "Gradient Maximum Intensity Projection" (GMIP) as described within the paper "Instant Volumetric Understanding with Order-Independent Volume Rendering" from B. Mora & D. S. Ebert
// -> Due to it's simplicity it's especially well suited to test the ray traversal in general for correctness


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform float Opacity;	// Opacity, usually within the interval [~0 .. 1] = [transparent .. solid]

/**
*  @brief
*    Integrates over the volume
*
*  @param[in] StartPosition
*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] NumberOfSteps
*    Number of steps along the ray (= number of samples to take), must be positive
*  @param[in] StepPositionDelta
*    Position advance per step along the ray, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] MaximumTravelLength
*    Maximum travel length along the ray, within the interval [0 .. 1]
*
*  @return
*    RGBA result of the ray traversal
*/
vec4 RayTraversal(vec3 StartPosition, int NumberOfSteps, vec3 StepPositionDelta, float MaximumTravelLength)
{
	// Current position inside the volume
	vec3 currentPosition = StartPosition;

	// Integrate over the volume and find the maximum gradient magnitude
	// -> Front-to-back traversal (traversal order doesn't matter for GMIP)
	float maximumGradientMagnitude = 0.0;
	vec3 maximumPosition = vec3(0.0, 0.0, 0.0);
	for (int step=0; step<NumberOfSteps; step++) {
		// Call the clip position function
		//     bool ClipPosition(vec3 Position)
		if (!ClipPosition(currentPosition)) {
			// Call the gradient function
			//     vec3 Gradient(vec3 Position)
			vec3 gradient = Gradient(currentPosition);

			// Calculate the magnitude of the gradient
			float gradientMagnitude = length(gradient);

			// New maximum scalar value?
			if (maximumGradientMagnitude < gradientMagnitude) {
				maximumGradientMagnitude = gradientMagnitude;
				maximumPosition          = currentPosition;
			}
		}

		// Update the current position inside the volume
		currentPosition += StepPositionDelta;
	}

	// Call the shading function
	//     vec4 Shading(float Scalar, vec3 Position, vec3 StepPositionDelta)
	vec4 color = Shading(maximumGradientMagnitude, maximumPosition, StepPositionDelta);

	// Apply the given opacity
	color.a *= Opacity;

	// Return the calculated fragment color
	return color;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
