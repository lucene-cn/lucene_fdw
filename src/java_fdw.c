#include <jni.h>
#define PATH_SEPARATOR ';' /* define it to be ':' on Solaris */
#define USER_CLASSPATH "." /* where Prog.class is */
// gcc lxjniapi.c -o lxjniapi -I /root/jdk-16.0.1/include/ -I /root/jdk-16.0.1/include/linux   -I /root/jdk-16.0.1/lib/server/ -L /root/jdk-16.0.1/lib/server/ -ljvm
char* Jstring2CStr(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = (*env)->FindClass(env, "java/lang/String");
	jstring strencode = (*env)->NewStringUTF(env, "utf-8");
	jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes",	"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid,	strencode);
	jsize alen = (*env)->GetArrayLength(env, barr);
	jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env, barr, ba, 0);
	return rtn;
}

main() {
    JNIEnv *env;
    JavaVM *jvm;
    jint res;
    jclass cls;
    jmethodID mid;
    jstring jstr;
    jclass stringClass;
    jobjectArray args;
#ifdef JNI_VERSION_1_2
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];
    options[0].optionString ="  -Xmx10g -Xms10g -XX:+UseZGC -Djava.class.path=" USER_CLASSPATH;
    vm_args.version = 0x00010002;
    vm_args.options = options;
    vm_args.nOptions = 1;
    vm_args.ignoreUnrecognized = JNI_TRUE;
 /* Create the Java VM */
    res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
#else
    JDK1_1InitArgs vm_args;
    char classpath[1024];
    vm_args.version = 0x00010001;
    JNI_GetDefaultJavaVMInitArgs(&vm_args);
    /* Append USER_CLASSPATH to the default system class path */
    sprintf(classpath, "%s%c%s", vm_args.classpath, PATH_SEPARATOR, USER_CLASSPATH);
    vm_args.classpath = classpath;
    /* Create the Java VM */
    res = JNI_CreateJavaVM(&jvm, &env, &vm_args);
#endif /* JNI_VERSION_1_2 */
    if (res < 0) {
        fprintf(stderr, "Can't create Java VM\n");
        goto destroy;
    }
    cls = (*env)->FindClass(env, "cn/lucene/fmdb/LxJniApi");
    if (cls == NULL) {
        goto destroy;
    }
  if(cls !=0) { 
   mid = (*env)->GetMethodID(env,cls,"<init>","()V"); 
     jobject jobj;   

   if(mid !=0){ 
     
    jobj=(*env)->NewObject(env,cls,mid); 
     
     fprintf(stderr, "init ok \n");

   }

    jstr = (*env)->NewStringUTF(env, " from C!");
    if (jstr == NULL) {
        goto destroy;
    }

     mid=(*env)->GetMethodID(env,cls,"pushdown","(Ljava/lang/String;)V");
    (*env)->CallVoidMethod(env,jobj,mid,jstr);

         mid=(*env)->GetMethodID(env,cls,"readLine","(Ljava/lang/String;)Ljava/lang/String;");

         	jstring line = (jstring)(*env)->CallObjectMethod(env,jobj, mid,jstr);

     char * cstr=Jstring2CStr(env,line);
     fprintf(stderr, cstr);
     fprintf(stderr, " \n");
     free (cstr) ;


     mid=(*env)->GetMethodID(env,cls,"readLine","(Ljava/lang/String;)Ljava/lang/String;");
    line = (jstring)(*env)->CallObjectMethod(env,jobj, mid,jstr);
     cstr=Jstring2CStr(env,line);
     fprintf(stderr, cstr);
     fprintf(stderr, " \n");
      free (cstr) ;

     mid=(*env)->GetMethodID(env,cls,"readLine","(Ljava/lang/String;)Ljava/lang/String;");
     line = (jstring)(*env)->CallObjectMethod(env,jobj, mid,jstr);
     cstr=Jstring2CStr(env,line);
     fprintf(stderr, cstr);
     fprintf(stderr, " \n");
       free (cstr) ;


destroy:
    if ((*env)->ExceptionOccurred(env)) {
        (*env)->ExceptionDescribe(env);
    }
    (*jvm)->DestroyJavaVM(jvm);
}




}