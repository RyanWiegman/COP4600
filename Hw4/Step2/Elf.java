package Step2;
import java.util.Random;

public class Elf implements Runnable {

	enum ElfState {
		WORKING, TROUBLE, AT_SANTAS_DOOR, DONE
	};
	private ElfState state;	 
	private int number;		 //The number associated with the Elf
	private Random rand = new Random();
	private SantaScenario scenario;
	private boolean terminate;

	public Elf(int number, SantaScenario scenario) {
		this.number = number;
		this.scenario = scenario;
		this.state = ElfState.WORKING;
		this.terminate = false;
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
			// wait a day
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			if(terminate){
				state = ElfState.DONE;
				return;
			}
				
			switch (state) {
				case WORKING:
					// at each day, there is a 1% chance that an elf runs into
					// trouble.
					if (rand.nextDouble() < 0.01) {
						state = ElfState.TROUBLE;
					}
					break;
				case TROUBLE:		// FIXME: if possible, move to Santa's door
					state = ElfState.AT_SANTAS_DOOR;
					scenario.atDoor.add(this);
					break;
				case AT_SANTAS_DOOR:	// FIXME: if feasible, wake up Santa
					scenario.santa.wakeSanta(1);
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
