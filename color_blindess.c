#ifndef __COLOR_BLINDNESS
#define __COLOR_BLINDNESS

#include <stdio.h>
#include <math.h>

void rgb2lms(unsigned char r, unsigned char  g, unsigned char b,
    float *l, float *m , float *s){

    float rr = (r / 255.0);
    float gg = (g / 255.0);
    float bb = (b / 255.0);

    // convert to srgb
    rr = rr <= 0.04045 ? (rr) / 12.92 : pow((rr + 0.055) / 1.055, 2.4);
    gg = gg <= 0.04045 ? (gg) / 12.92 : pow((gg + 0.055) / 1.055, 2.4);
    bb = bb <= 0.04045 ? (bb) / 12.92 : pow((bb + 0.055) / 1.055, 2.4);


    // convert to lms
    *l = (rr * 0.31399022) +  (gg * 0.15537241) + (bb * 0.01775239);
    *m = (rr * 0.63951294) +  (gg * 0.75789446) + (bb * 0.10944209);
    *s = (rr * 0.04649755) +  (gg * 0.08670142) + (bb * 0.87256922);
}

void lms2rgb(float l, float m, float s,
    unsigned char *r, unsigned char  *g, unsigned char *b) {

    // convert to srgb
    float rr = (l *  5.47221206) + (m * -1.1252419   ) + (s * 0.02980165);
    float gg = (l * -4.6419601)  + (m *   2.29317094 ) + (s *   -0.19318073);
    float bb = (l *  0.16963708) + (m * -0.1678952   ) + (s * 1.16364789);

    // convert to rgb
    rr = rr <= 0.0031308 ? rr * 12.92 : (pow(1.055 * rr, 0.41666) - 0.055);
    gg = gg <= 0.0031308 ? gg * 12.92 : (pow(1.055 * gg, 0.41666) - 0.055);
    bb = bb <= 0.0031308 ? bb * 12.92 : (pow(1.055 * bb, 0.41666) - 0.055);

    rr *=255;
    gg *=255;
    bb *=255;
  
    // clamp value
    *r = (rr < 0)? 0:(rr>255)? 255:rr;
    *g = (gg < 0)? 0:(gg>255)? 255:gg;
    *b = (bb < 0)? 0:(bb>255)? 255:bb;
}


enum cvd_type{
    protanopia,
    deuteranopia,
    tritanopia,
};

enum cvd_operation{
    simualte,
    correct,
};

void simulate_protanopia(float l, float m, float s, float *ll, float *mm, float *ss) {
	*ll = (0.0 * l)	+ (2.02344 * m) + (-2.52581 * s);
	*mm = (0.0 * l) 	+ (1.0 * m) 	+ (0.0 * s);
	*ss = (0.0 * l) 	+ (0.0 * m) 	+ (1.0 * s);
    
    return;
}
void simulate_deuteranopia(float l, float m, float s, float *ll, float *mm, float *ss) {
	*ll = (1.0 * l) 		+ (0.0 * m)	+ (0.0 * s);
	*mm = (0.494207 * l) + (0.0 * m)	+ (1.24827 * s);
	*ss = (0.0 * l) 		+ (0.0 * m)	+ (1.0 * s);

    return;
}
void simulate_tritanopia(float l, float m, float s, float *ll, float *mm, float *ss) {
	*ll = (1.0 * l) 			+ (0.0 * m) 		+ (0.0 * s);
	*mm = (0.0 * l) 			+ (1.0 * m) 		+ (0.0 * s);
	*ss = (-0.395913 * l) 	+ (0.801109 * m) 	+ (0.0 * s);

    return;
}

void correct_protanopia(float l, float m, float s, float *ll, float *mm, float *ss) {
	*ll = (0.0 * l) + (2.02344 * m) + (-2.52581 * s);
	*mm = (0.0 * l) + (1.0 * m) + (0.0 * s);
	*ss = (0.0 * l) + (0.0 * m) + (1.0 * s);
}
void correct_deuteranopia(float l, float m, float s, float *ll, float *mm, float *ss) {
	*ll = (1.0 * l) 		+ (0.0 * m) + (0.0 * s);
	*mm = (0.494207 * l) + (0.0 * m) + (1.24827 * s);
	*ss = (0.0 * l)		+ (0.0 * m) + (1.0 * s);
}
void correct_tritanopia(float l, float m, float s, float *ll, float *mm, float *ss) {
	*ll = (1.0 * l) 			+ (0.0 * m) 		+ (0.0 * s);
	*mm = (0.0 * l) 			+ (1.0 * m) 		+ (0.0 * s);
	*ss = (-0.395913 * l) 	+ (0.801109 * m) 	+ (0.0 * s);

}

#endif // #ifndef __COLOR_BLINDNESS
