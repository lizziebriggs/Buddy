using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "Plant", menuName = "Plants/New Plant", order = 1)]
public class Plant : ScriptableObject
{
    [Header("Information")]
    public string plantName;

    [Header("Conditions")]
    public float recTempMin;
    public float recTempMax, recWaterMin, recWaterMax, recLightMin, recLightMax;

}
