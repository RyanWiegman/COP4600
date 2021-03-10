//import com.sun.org.apache.xml.internal.security.utils.HelperNodeList;
package Step2;

import Step2.Elf.ElfState;

public class Santa implements Runnable {

	enum SantaState {SLEEPING, READY_FOR_CHRISTMAS, WOKEN_UP_BY_ELVES, WOKEN_UP_BY_REINDEER, DONE};
	private SantaState state;
	private boolean terminate;
	private SantaScenario scenario;
	
	public Santa(SantaScenario scenario) {
		this.state = SantaState.SLEEPING;
		this.terminate = false;
		this.scenario = scenario;
	}

	public void setTerminate(boolean terminate) {
		this.terminate = terminate;
	}
	
	public void wakeSanta(int reason){
		if(reason == 1)
			state = SantaState.WOKEN_UP_BY_ELVES;
		else
			state = SantaState.WOKEN_UP_BY_REINDEER;
	}

	@Override
	public void run() {
		while(true) {
			if(terminate){
				state = SantaState.DONE;
				return;
			}
			// wait a day...
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			switch(state) {
			case SLEEPING: // if sleeping, continue to sleep
				break;
			case WOKEN_UP_BY_ELVES: 	// FIXME: help the elves who are at the door and go back to sleep 
				for(Elf elf : scenario.atDoor){
					elf.setState(ElfState.WORKING);
					//scenario.atDoor.remove(elf);
				}
				state = SantaState.SLEEPING;
				
				break;
			case WOKEN_UP_BY_REINDEER: 
				// FIXME: assemble the reindeer to the sleigh then change state to ready 
				break;
			case READY_FOR_CHRISTMAS: // nothing more to be done
				break;
			}
		}
	}

	
	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Santa : " + state);
		System.out.println("arraylist size: " + scenario.atDoor.size());
	}
	
	
}
