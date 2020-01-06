#include <jni.h>
#include "net_natpad_brilliance_caterpillar_Caterpillar.h"
#include <caterpillar.h>


/*
 * Class:     net_natpad_brilliance_caterpillar_Caterpillar
 * Method:    referenceReport
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_caterpillar_Caterpillar_referenceReport
  (JNIEnv *env, jclass cls) {
	cat_ref_report();
	cat_ref_entry_report();
}
