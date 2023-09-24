package main

import (
	"fmt"
	"image/color"
	"os"
	"regexp"
	"strconv"
	"strings"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
)

type commaTicks struct{}

func addCommas(s string) string {
	rev := ""
	n := 0
	for i := len(s) - 1; i >= 0; i-- {
		rev += string(s[i])
		n++
		if n%3 == 0 {
			rev += ","
		}
	}
	s = ""
	for i := len(rev) - 1; i >= 0; i-- {
		s += string(rev[i])
	}
	return s
}

func (commaTicks) Ticks(min, max float64) []plot.Tick {
	tks := plot.DefaultTicks{}.Ticks(min, max)
	for i, t := range tks {
		if t.Label == "" {
			continue
		}
		tks[i].Label = addCommas(t.Label)
	}
	return tks
}

func readFromFile(filename string) ([]byte, error) {
	data, err := os.ReadFile(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
	}
	return data, err
}

func Ploteo(insertion, merge []float64, dim int) {
	p := plot.New()
	p.Title.Text = "InsertionSort vs MergeSort"
	p.X.Label.Text = "Size of Data"
	p.Y.Label.Text = "Delay Time"
	p.Y.Tick.Marker = commaTicks{}
	ptsInsertion := make(plotter.XYs, len(insertion))
	ptsMerge := make(plotter.XYs, len(merge))
	for i := 0; i < len(insertion); i++ {
		ptsInsertion[i].X = float64(i)
		ptsInsertion[i].Y = insertion[i]

		ptsMerge[i].X = float64(i)
		ptsMerge[i].Y = merge[i]
	}
	lineInsertion, err := plotter.NewLine(ptsInsertion)
	if err != nil {
		panic(err)
	}
	lineInsertion.LineStyle.Width = vg.Points(1)
	lineInsertion.LineStyle.Color = color.RGBA{R: 0, G: 0, B: 255, A: 255}

	lineMerge, err := plotter.NewLine(ptsMerge)
	if err != nil {
		panic(err)
	}
	lineMerge.LineStyle.Width = vg.Points(1)
	lineMerge.LineStyle.Color = color.RGBA{R: 0, G: 255, B: 0, A: 255}
	legend := plot.NewLegend()
	legend.Add("InsertionSort", lineInsertion)
	legend.Add("MergeSort", lineMerge)
	legend.Top = true
	legend.Left = true
	p.Legend = legend
	p.Add(lineInsertion, lineMerge)
	if err := p.Save(6*vg.Inch, 4*vg.Inch, "Lineal.png"); err != nil {
		panic(err)
	}
	fmt.Println("Grafico Lineal guardado en 'Lineal.png'")
}

func SplitLogLine(text string) []float64 {
	re := regexp.MustCompile(`\s+`)
	parts := re.Split(text, -1)
	var numbers []float64

	for _, part := range parts {
		part = strings.TrimSpace(part)
		if part != "" {
			number, err := strconv.ParseFloat(part, 64)
			if err != nil {
				fmt.Printf("Error al analizar número '%s': %v\n", part, err)
			} else {
				numbers = append(numbers, number)
			}
		}
	}
	return numbers
}

func main() {
	data1, err1 := readFromFile("Lineal/Insertion")
	if err1 != nil {
		return
	}
	data2, err2 := readFromFile("Lineal/Merge")
	if err2 != nil {
		return
	}
	//fmt.Println("Received:", string(data))
	insertion := SplitLogLine(string(data1))
	merge := SplitLogLine(string(data2))
	Ploteo(insertion, merge, len(insertion))
}
