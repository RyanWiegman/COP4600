package Step5;

import java.util.Random;

public class Reindeer implements Runnable {

	public enum ReindeerState {AT_BEACH, AT_WARMING_SHED, AT_THE_SLEIGH};
	private ReindeerState state;
	private SantaScenario scenario;
	private Random rand = new Random();
	private boolean terminate;
	private int number;		//The number associated with the reindeer
	
	public Reindeer(int number, SantaScenario scenario) {
		this.number = number;
		this.scenario = scenario;
		this.state = ReindeerState.AT_BEACH;
		this.terminate = false;
	}

	public void setState(ReindeerState state) {
		this.state = state;
	}

	public void setTerminate(boolean terminate) {
		this.terminate = terminate;
	}

	@Override
	public void run() {
		while(true) {
			if(terminate)
				return;
				
			// wait a day
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// see what we need to do:
			switch(state) {
				case AT_BEACH: { // if it is December, the reindeer might think about returning from the beach
					if (scenario.isDecember) {
						if (rand.nextDouble() < 0.1) {
							state = ReindeerState.AT_WARMING_SHED;
							scenario.warming_shed_ctr++;
						}
					}
					break;			
				}
				case AT_WARMING_SHED: // if all the reindeer are home, wake up santa
				try {
					scenario.reindeer_sem.acquire();
					scenario.santa.wakeSanta(0);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
					break;
				case AT_THE_SLEIGH: // keep pulling
					break;
			}
		}
	}
	
	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Reindeer " + number + " : " + state);
	}
	
}
