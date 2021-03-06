package Step3;
import java.util.Random;

import Step3.Santa.SantaState;

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
				inTrouble = false;
				if (rand.nextDouble() < 0.01) {
					state = ElfState.TROUBLE;
					System.out.println("\nlist day in trouble" + scenario.tempDay);
				}
				break;
			case TROUBLE:		// FIXME: if possible, move to Santa's door
				if(!inTrouble){
					scenario.inTroubleList.add(this);
					inTrouble = true;
				}
					
				break;
			case AT_SANTAS_DOOR:	// FIXME: if feasible, wake up Santa
					//scenario.santa.wakeSanta(1);
					//scenario.santa.setState(SantaState.WOKEN_UP_BY_ELVES);		takes an extra day for santa to wake
				break;
			}
		}
	}

	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Elf " + number + " : " + state + ", introuble: " + inTrouble);
	}

}
