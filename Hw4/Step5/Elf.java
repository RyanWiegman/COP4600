package Step5;
import java.util.Random;

public class Elf implements Runnable {

	enum ElfState {
		WORKING, TROUBLE, AT_SANTAS_DOOR, DONE
	};
	private ElfState state;	 
	public int number;		 //The number associated with the Elf
	private Random rand = new Random();
	private SantaScenario scenario;
	private boolean terminate;
	private boolean inTrouble;

	public Elf(int number, SantaScenario scenario) {
		this.number = number;
		this.scenario = scenario;
		this.state = ElfState.WORKING;
		this.terminate = false;
		this.inTrouble = false;
	}


	public ElfState getState() {
		return state;
	}

	/**
	 * Santa might call this function to fix the trouble
	 * @param state
	 */
	public void setState(ElfState state) {
		this.state = state;
	}

	public void setTerminate(boolean terminate) {
		this.terminate = terminate;
	}

	@Override
	public void run() {
		while (true) {
			if(terminate){
				state = ElfState.DONE;
				return;
			}
				
			// wait a day
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			switch (state) {
				case WORKING:
					// at each day, there is a 1% chance that an elf runs into
					// trouble.
					if (rand.nextDouble() < 0.01) {
						state = ElfState.TROUBLE;
						scenario.inTroubleList.add(this);
					}
					break;
				case TROUBLE:		// FIXME: if possible, move to Santa's door
					try {
						scenario.santa_door_check.acquire();
						int troubleList_delete = scenario.inTroubleList.size() - 1;
						int index = 0;
						while(scenario.inTroubleList.size() != 0){
							scenario.atDoor.add(scenario.inTroubleList.get(troubleList_delete));
							scenario.atDoor.get(index).setState(Elf.ElfState.AT_SANTAS_DOOR);
							scenario.inTroubleList.remove(troubleList_delete);
							index++;
							troubleList_delete--;
						}
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					break;
				case AT_SANTAS_DOOR:	// FIXME: if feasible, wake up Santa
					if(scenario.atDoor.size() == 3)
						scenario.santa.state = Santa.SantaState.WOKEN_UP_BY_ELVES;
					break;
			}
		}
	}

	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Elf " + number + " : " + state);
	}

}
