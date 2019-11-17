//---------------------------------------------------------------------------
// --------------------------- Shell.java------------------------------------
// Angie(Nomingerel) Tserenjav
// CSS 430 Section A
// Creation Date: 08/10/19
// Date of Last Modification: 13/10/19
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// Purpose: This program is to help understand the kernel's view point, the shell
// is simply viewed as an application program that uses system calls to spawn and 
// to terminate other user programs. 
// --------------------------------------------------------------------------
//---------------------------------------------------------------------------
public class Shell extends Thread{
	public Shell(){
	}
	public void run(){
	    // starts the count 1
	    int lineCount = 1;
	    while(true){
		StringBuffer bf = new StringBuffer();
		//display the My shell
		SysLib.cout("My shell[" + lineCount + "]% ");
		SysLib.cin(bf);
		String line = bf.toString();
	    // compare the strin with exit 
            if (line.compareTo("exit") == 0){
                break;
            }
	// created result1 is to split by ;
	String[] result1 = line.split(";");
	// created result2 is to split by &
	String[] result2 = line.split("&");
	// checking the conditions 
         if (result1.length > 1 && result2.length > 1){ 
                multi(result1); 
            } else if (result2.length > 1) { 
                handler1(result2); 
            } else { 
                handler2(result1); 
            }
            lineCount++;
        }
	// exit from the shell
        SysLib.exit();
    }
    // if user enteres the job with ; or & then call multi method to run 
    public void multi(String[] args) {
        for (int i = 0; i < args.length; i++) {
            SysLib.cout(args[i] + "\n");
            String[] result1= args[i].split("&");
            if (result1.length > 1) { 
                handler1(result1);
            } else {
                String[] arg= SysLib.stringToArgs(args[i]);
                if (SysLib.exec(arg) < 0) {
                    return;
                }
                SysLib.join();
            }
        }
    }

    // if the user enters the string with split ; then it will call the handler2 method
    public void handler2(String[] args) {
        for (int i = 0; i < args.length; i++) {
            String[] hd2 = SysLib.stringToArgs(args[i]);
            SysLib.cout(hd2[0] + "\n");
            if (SysLib.exec(hd2) < 0) {
                return;
            }
            SysLib.join(); 
        }
    }

    // if the user enters the string with split & then it will call the handler1 method
    public void handler1(String[] args) {
        int hand = 0;
        for (int i = 0; i < args.length; i++) {
            String[] hd1= SysLib.stringToArgs(args[i]);
            SysLib.cout(hd1[0] + "\n");
            if (SysLib.exec(hd1) < 0) {
            } else {
                hand++;
            }
        }
        for (int i = 0; i < curr; i++) {
            SysLib.join(); 
        }
    }
}