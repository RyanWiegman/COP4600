//import com.sun.org.apache.xml.internal.security.utils.HelperNodeList;
package Step4;

public class Santa implements Runnable {

	enum SantaState {SLEEPING, READY_FOR_CHRISTMAS, WOKEN_UP_BY_ELVES, WOKEN_UP_BY_REINDEER, DONE};
	public SantaState state;
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

	public void setState(SantaState state) {
		this.state = state;
	}
	
	public void wakeSanta(int reason){
		if(state == SantaState.SLEEPING){
			if(reason == 1)
				state = SantaState.WOKEN_UP_BY_ELVES;
			else
				state = SantaState.WOKEN_UP_BY_REINDEER;
		}
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
					for(int index = 0; index < scenario.atDoor.size(); index++)
						scenario.atDoor.get(index).setState(Elf.ElfState.WORKING);
					scenario.atDoor.clear();
					if(scenario.atDoor.isEmpty())
						System.out.println("santa state: " + state + "\n atDoor list in empty.\n");
					else
						System.out.println("atDoor list not empty when should be. \n");
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
	}
	
	
}
