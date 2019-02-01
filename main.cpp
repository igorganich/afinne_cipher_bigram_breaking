#include <iostream>
#include <fstream>
#include <string>

using namespace std;

wchar_t *alphabet = L"абвгдежзийклмнопрстуфхцчшщыьэюя";
int get_from_alphabet(wchar_t c)
{
	for (int i = 0; i < 31; i++)
		if(c == alphabet[i])
			return (i);
	printf("error\n");
	return (-1);
}

int *create_bigrams(wstring str, int *texted_bigram)
{
	int cur = 0;
	int flag = 0;
	int *bigram = new int[961];
	for (int i = 0; i < 961; i++)
		bigram[i] = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (flag == 0)
		{
			cur = get_from_alphabet(str[i]);
			cur *= 31;
			flag++;
		}
		else
		{
			flag = 0;
			cur += get_from_alphabet(str[i]);
			bigram[cur]++;
			texted_bigram[i / 2] = cur;
		}
	}
	return(bigram);
}

int *get_best_bigrams(int *bigrams)
{
	int *candidates = new int[5];
	int best;
	for (int i = 0; i < 5; i++)
	{
		best = 0;
		for (int l = 0; l < 961; l++)
			if (bigrams[l] > bigrams[best])
				best = l;
		candidates[i] = best;
		bigrams[best] = 0;
	}
	return (candidates);
}

int *create_rus(void)
{
	int *rus = new int[5];
	rus[0] = get_from_alphabet(L'с') * 31 + get_from_alphabet(L'т');
	rus[1] = get_from_alphabet(L'н') * 31 + get_from_alphabet(L'о');
	rus[2] = get_from_alphabet(L'т') * 31 + get_from_alphabet(L'о');
	rus[3] = get_from_alphabet(L'н') * 31 + get_from_alphabet(L'а');
	rus[4] = get_from_alphabet(L'е') * 31 + get_from_alphabet(L'н');
	return (rus);
}

void normal(int *num)
{
	if (*num < 0)
		*num = 961 + *num;
}

int gcd(int razx, int temp)
{
	return ((temp == 0) ? razx : gcd(temp, razx % temp));
}

int calc(int a, int b, int *x, int *y)
{
	int q, r, x1, x2, y1, y2;
	x2 = 1; x1 = 0; y2 = 0; y1 = 1;
	if (b == 0)
	{
		*x = 1;
		*y = 0;
		return (a);
	}
	while (b > 0)
	{
		q = a / b;
		r = a - q * b;
		*x = x2 - q * x1;
		a = b;
		b = r;
		x2 = x1, x1 = *x, y2 = y1, y1 = *y;
	}
	*x = x2;
	*y = y2;
	return (a);
}

int evklid(int a, int n)
{
	int d, x, y;
	d = calc(a, n, &x, &y);
	if (d == 1)
	{
		if (x > 0)
			return x;
		else
			return (n + x);
	}
	return (0);

}

int *get_all_a(int *a_size, int razx, int razy)
{
	normal(&razx);
	normal(&razy);
	int min = gcd(razx, 961);
	int *ashki = new int[min];
	if (min == 1)
	{
		*a_size = 1;
		ashki[0] = (evklid(razx, 961) * razy) % 961;
		normal(&(ashki[0]));
	}
	else if (razy % min == 0)
	{
		cout << "zashlo\n";
		razx /= min;
		razy /= min;
		int templat = evklid(razx, 961 / min) * razy;
		normal(&templat);
		for (int i = 0; i < min; i++)
		{
			*(a_size)++;
			ashki[i] = (templat + i * 961)%961;
			normal(&(ashki[i]));
		}
	}
	return (ashki);
}

double index(wchar_t *text, int size)
{
	int count[31];
	for (int i = 0; i < 31; i++)
		count[i] = 0;
	for (int i = 0; i < size; i++)
	{
		int cur = get_from_alphabet(text[i]);
		count[cur]++;
	}
	double ret = 0;
	for (int i = 0; i < 31; i++)
	{
		double first = (double)(count[i] * (count[i] - 1));
		double second = first / (double) (size * (size - 1));
		ret += second;
	}
	return (ret);
}

int check_text(int *ashki, int *bshki, int a_size, int *texted_bigram, int text_size)
{
	wchar_t *tmp = new wchar_t[text_size * 2 + 1];
	tmp[text_size * 2] = 0;
	for (int cur_a = 0; cur_a < a_size; cur_a++)
	{
		if (evklid(ashki[cur_a], 961) == 0)
			continue;
		for (int i = 0; i < text_size; i++)
		{
			int x = (evklid(ashki[cur_a], 961) * (texted_bigram[i] - bshki[cur_a]))%961;
			normal(&x);
			tmp[i * 2 + 1] = alphabet[x % 31];
			tmp[i * 2] = alphabet[(x - (x % 31)) / 31];
		}
		double ind = index(tmp, text_size * 2);
		wcout << ind << endl;
		if (index(tmp, text_size * 2) > 0.055)
		{
			wcout << ashki[cur_a] << " " << bshki[cur_a] << endl;
			wcout <<tmp << endl << endl << endl;
			delete tmp;
			return (1);
		}
	}
	delete tmp;
	return (0);
}

int *get_all_b(int size, int *ashki, int rus, int txt)
{
	int *bshki = new int[size];
	for (int i = 0; i < size; i++)
	{
		bshki[i] = (txt - ashki[i] * rus)%961;
		normal(&(bshki[i]));
	}
	return (bshki);
}

int main()
{
	setlocale(LC_ALL, "");
	wfstream file("V12");
	wstring str;
	locale loc("");
	wcout.imbue(loc);
	file.imbue(loc);
	getline(file, str);
	wcout << str << endl;
	int *texted_bigram = new int[str.size() / 2];
	int *bigrams = create_bigrams(str, texted_bigram);
	int *candidates = get_best_bigrams(bigrams);
	int *rus_candidates = create_rus();
	int i = -1, l = -1, j = -1, k = -1;
	int flag = 0;
	while (++i < 5 && (l = -1) && flag == 0)
	{
		while (++l < 5 && (j = -1) && flag == 0)
		{
			while (++j < 5 && (k = -1) && flag == 0)
			{
				if (j == i)// nelzya combinit' s soboy
					continue;
while (++k < 5 && flag == 0)
{
	if (j == k)// tut toje samoe
		continue;
	int a_size = 0;
	int *ashki = get_all_a(&a_size, rus_candidates[i] - rus_candidates[j], candidates[l] - candidates[k]);
	int *bshki = get_all_b(a_size, ashki, rus_candidates[i], candidates[l]);
	if (check_text(ashki, bshki, a_size, texted_bigram, (str.size() / 2)) == 1)
		flag = 1;
	delete ashki;
	delete bshki;
}
			}
		}
	}
	delete bigrams;
	delete candidates;
	delete rus_candidates;
	return (1);
}
