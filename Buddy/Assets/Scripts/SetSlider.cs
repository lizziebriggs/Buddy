using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SetSlider : MonoBehaviour
{
    [SerializeField] private Slider slider;
    [SerializeField] private InputField minInput, maxInput;

    public void SetMin()
    {
        slider.minValue = float.Parse(minInput.text);
    }

    public void SetMax()
    {
        slider.maxValue = float.Parse(maxInput.text);
    }
}
