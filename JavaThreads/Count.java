/**
 * Count.java
 * A class that can be run as a Thread to print out sequential numbers
 */
public class Count extends Thread {
    /**
     * A consideration in the design of a threaded program in Java is that, unlike C, the
     * run() method that actually starts the thread doesn't take any parameters.
     * 
     * Therefore all of the data the thread actually needs must already exist inside of it,
     * or be connected to it as a reference.  If whatever you need is outside the scope of
     * this object, it will be inaccessible to the thread.
     */

    // Class-level integer, used to determine the starting point for printing numbers.
    private int num;

    /**
     * The default constructor for this Count object takes the "num" parameter
     * and stores it in the class-level "num" field.
     * 
     * @param num 
     */
    public Count(int num) {
        this.num = num;
    }

    /**
     * The Thread class implements the Runnable interface.
     * Remember that in Java, implementing an interface means that the class must have
     * a version of all of the methods defined by the interface.
     * The Runnable interface has one required method, which is <strong>run</strong>.
     * It uses this method to tell the created thread what to do, so by overriding run()
     * and giving it instructions, this is what will actually run in our threads.
     */
    @Override
    public void run() {
        // This is a pretty simple method.  It starts at the starting point defined in num above,
        // and prints out the next five numbers.
        for (int i = num; i < num + 5; i++) {
            System.out.print(i + " ");
        }
        System.out.println();
    }
}