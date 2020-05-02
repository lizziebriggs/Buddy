using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[CreateAssetMenu(fileName = "Plant", menuName = "Plants/New Plant", order = 1)]
public class Plant : ScriptableObject
{
    [Header("Information")]
    public string plantName;
    // Date

    [Header("Conditions")]
    public float recTempMin;
    public float recTempMax, recWaterMin, recWaterMax, recLightMin, recLightMax;

    [Header("Facts")]
    public Image image;
    [TextArea] public string about;
    [TextArea] public string howToCare;
    [TextArea] public string family;
}
