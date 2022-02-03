package gui.action;

import java.io.PrintStream;
import java.util.TreeMap;

public class ActionData {
	public ActionType type;
	public ActionButtonPressType pressType = null;
	public Object handlingData = null;
	
	public ActionData(ActionType type) {
		this.type = type;
	}
	
	public ActionData(ActionType type, Object objData) {
		this.type = type;
		this.handlingData = objData;
	}
	
	private static TreeMap<ActionType, String> ad_mapTypeClaims = new TreeMap<ActionType, String>();
	
	public static boolean ClaimActionType(ActionType atType, String strName) {
		// deny claim if already claimed
		if (ad_mapTypeClaims.containsKey(atType)) {
			return false;
		}
		
		// claim the action type
		ad_mapTypeClaims.put(atType, strName);
		return true;
	}
	
	public static String CheckTypeClaim(ActionType atType) {
		// null if not claimed
		if (!ad_mapTypeClaims.containsKey(atType)) {
			return null;
		}
		
		// otherwise return the claim name
		return ad_mapTypeClaims.get(atType);
	}
	
	public static void TraceTypeClaims(PrintStream psOutput) {
		for (ActionType atKey : ad_mapTypeClaims.keySet()) {
			psOutput.printf("Type %s claimed for: \"%s\"\n", atKey, ad_mapTypeClaims.get(atKey));
		}
	}
}
