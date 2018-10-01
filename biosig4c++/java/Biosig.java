/* 
  https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html
*/
//package Biosig;
public class Biosig {
    static { 
    	System.loadLibrary("Biosig"); 
    }

    private native void version();
    private native String hdr2ascii(String filename);
    private native void hdr2json(String filename);
    
    private native void sopen(String filename);
    private native void sread(String filename);
    private native void sclose(String filename);

    public static void main(String[] args) {
         System.out.println(new Biosig().hdr2ascii(args[0]));
    }
}


