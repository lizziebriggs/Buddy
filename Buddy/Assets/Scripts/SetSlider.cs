using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SetSlider : MonoBehaviour
{
    [SerializeField] private AppManager appManager;

    [SerializeField] private Slider slider;
    [SerializeField] private InputField minInput, maxInput;
    private float currentMin, currentMax;

    void Start()
    {
        appManager = FindObjectOfType<AppManager>();

        currentMin = float.Parse(minInput.text);
        currentMax = float.Parse(maxInput.text);
    }

    public void SetMin()
    {
        try
        {
            float newValue = float.Parse(minInput.text);

            if (newValue >= currentMax)
            {
                minInput.text = currentMin.ToString();
                appManager.DisplayError("Value too big! This must be less than the max value.");
                return;
            }

            currentMin = newValue;
            slider.minValue = currentMin;
        }

        catch (System.FormatException)
        {
            minInput.text = currentMin.ToString();
            appManager.DisplayError("Invalid input! You must enter a number.");

            throw;
        }
    }

    public void SetMax()
    {
        try
        {
            float newValue = float.Parse(minInput.text);

            if (newValue <= currentMin)
            {
                maxInput.text = currentMax.ToString();
                appManager.DisplayError("Value too small! This must be greater than the min value.");
                return;
            }

            currentMax = newValue;
            slider.maxValue = currentMax;
        }

        catch (System.FormatException)
        {
            maxInput.text = currentMax.ToString();
            appManager.DisplayError("Invalid input! You must enter a number.");

            throw;
        }
    }
}
